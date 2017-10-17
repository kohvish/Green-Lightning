// pre-made libraries to be included
#include <allegro.h>
#include <stdlib.h>
#include <cstdlib>

//defining the y-coodinate of the sprite used in game
#define left 0
#define right 22
#define down 46
#define up 65


// this function will show the user game-credit when the user presses 'c'
void show_credit(){

    BITMAP* buffer = create_bitmap(800, 600);
    clear_bitmap(buffer);
    BITMAP *credit = load_bitmap("credit.tga",NULL);

    blit(credit,buffer,0,0,0,0,800,600);
    blit(buffer,screen,0,0,0,0,800,600);

    bool done=false;

    // go back to menu only is escape is pressed.
    while(done==false){
        clear_keybuf();
        if(key[KEY_ESC]){
            done=true;
        }
    }

    // memory management
    destroy_bitmap(credit);
    destroy_bitmap(buffer);

    return;

}

// show the user controls for the game
void show_help(){

    //Load the correct picture and play the correct sound
    //Signals that the function has worked perfectly.

    SAMPLE *help_sound=load_sample("help.wav");
    play_sample(help_sound,255,128,1000,false);

    BITMAP *buffer = create_bitmap(800,600);
    BITMAP *help_screen = load_bitmap("help_screen.tga",NULL);
    clear(buffer);

    blit(help_screen,buffer,0,0,60,20,800,600);
    blit(buffer,screen,0,0,0,0,800,600);

    // go back only if escape is pressed
    bool done =false;
    while(done==false){
        if(key[KEY_ESC]){done=true;}
    }

    //destroy all the resources that are of no more use.
    destroy_bitmap(help_screen);
    destroy_bitmap(buffer);
    destroy_sample(help_sound);

    return;
}

// this is where the game loop starts
void start_game(){

    //create bitmap and load all images and sound
    BITMAP *buffer = create_bitmap(800,600);
    clear(buffer);

    PALETTE palette;// Palette variable for Windows/OS2 8,16,24 or 32 bit BMP

    BITMAP *snake = load_bitmap("snake.tga", NULL);
    BITMAP *grass = load_bmp("grassx.bmp",palette);
    BITMAP *movement = load_bitmap("snake_cropped.tga",NULL);

    if(snake == 0) {allegro_message("Snake sprite could not be loaded"); exit(1);}
    if(grass == 0) {allegro_message("Grass sprite could not be loaded"); exit(1);}
    if(movement == 0) {allegro_message("snake_cropped sprite could not be loaded"); exit(1);}


    SAMPLE *game_sound = load_sample("gameplay.wav");

    play_sample(game_sound,150,128,800,true);

    //pre-define variables used throughout the game
    int rest_time = 70, image_x = 0, image_y=right, x=400,y=300, c_snake_x = 600, c_snake_y = 500, c_image_x=0, c_image_y=right;
    int fruit_x=300, fruit_y=200,score=0, direction=0,multiplier=1, poison_x,poison_y;
    bool poison=false, caught = false;

    // setup the initial snake and the fruit
    blit(grass, buffer, 0, 0, 0, 0, 800, 600);
    masked_blit(snake, buffer, 0,0 ,400,300,22,22);
    blit(buffer, screen, 0, 0, 0, 0, 800, 600);

    clear_keybuf();


    //press any key to start the game
    while(!keypressed()){}

    bool done = false, play_game_music=true;;

    // loop breaks when the game ends
    while (done==false){

        clear_keybuf();

        if((key[KEY_LCONTROL] && key[KEY_M]) || (key[KEY_RCONTROL] && key[KEY_M])){

            if (play_game_music==true){
                stop_sample(game_sound);

                play_game_music=false;
            }
            else{
                play_sample(game_sound,150,128,1000,true);
                play_game_music=true;
            }

        }

        // different pre-defined key controls
        if((key[KEY_LCONTROL] && key[KEY_H]) || (key[KEY_RCONTROL] && key[KEY_H])){
            show_help();
            rest(222);
            clear_keybuf();
        }


        if (key[KEY_ESC]){
            done=true;
            continue;
        }
        else if (key[KEY_LEFT]){
            image_y=left;
            direction = 0;
        }
        else if (key[KEY_RIGHT]){
            image_y=right;
            direction=1;
        }
        else if (key[KEY_DOWN]){
            image_y= down;
            direction = 2;
        }
        else if (key[KEY_UP]){
            image_y = up;
            direction =3;
        }

        // blit the next part of the image if no key is pressed
        if (!key[KEY_LEFT] && !key[KEY_RIGHT] && !key[KEY_DOWN] && !key[KEY_UP]){
            image_x=image_x+25;
        }

        //required for cycling of snake images.
        if (image_x>=75){
            image_x=0;
        }

        //changing x and y to give the illusion that the snake is moving
        if (direction==0){
            x=x-5;
        }
        else if (direction ==1){
            x=x+5;
        }
        else if (direction == 2){
            y=y+5;
        }
        else if(direction == 3){
            y=y-5;
        }

        // if the snake collides with any wall/
        // play sound and end the game with the correct sound and picture.
        if (x>=800 || x<=0 || y>=600 ||y<=0){


            if (play_game_music==true){
                stop_sample(game_sound);
            }


            SAMPLE *fail_sound=load_sample("fail.wav");
            play_sample(fail_sound,255,128,100,false);

            BITMAP *over=create_bitmap(800,600);
            over = load_bitmap("game_over.tga",NULL);
            blit(over,buffer,50,65,0,0,800,600);
            blit(buffer,screen,0,0,0,0,800,600);
            done=true;

            rest(4200);
            destroy_bitmap(over);
            destroy_sample(fail_sound);

        }

        //generate x2 and y2 for the snake and the fruit
        int fruit_x2 = fruit_x+18;
        int fruit_y2 = fruit_y+17;

        int snake_x2 = x+21;
        int snake_y2 = y+21;



        // check if the snake and any other bitmap overlap.
        if (snake_x2<fruit_x || fruit_x2 < x || snake_y2 < fruit_y || fruit_y2 < y) {}
        else {

            SAMPLE *eaten = load_sample("bite.wav");
            play_sample(eaten,255,128,100,false);
            destroy_sample(eaten);

            multiplier=multiplier+1;

            bool generated = false;
            while(generated==false){

                int temp_x, temp_y;

                temp_x = 50+ rand() % 750;
                temp_y = 50+ rand() % 550;

                if (temp_x!= fruit_x && temp_y!=fruit_y){

                    fruit_x=temp_x;
                    fruit_y=temp_y;
                    generated = true;

                    score=score+10;
                }

            }

            //required for blitting the poison on the screen.
            if (multiplier==6){
                multiplier=1;
                bool generated=false;

                while(generated==false){
                    int temp_x = 50+ rand() % 750;
                    int temp_y = 50+ rand() % 550;

                    if(temp_x!=fruit_x && temp_y!=fruit_y){
                        poison_x=temp_x;
                        poison_y=temp_y;
                        poison=true;
                        generated=true;
                    }
                }

            }


            rest_time=rest_time-5;

            //variable rest time to change the snake speed.
            if (rest_time<=25){
                rest_time=25;

            }

        }


        // check if the snake ate the poison and take it off the screen.
        if (snake_x2<poison_x || (poison_x+21) < x || snake_y2 < poison_y || (poison_y+16) < y) {}
        else {

            rest_time=rest_time+15;
            poison_x=NULL;
            poison_y=NULL;
            poison=false;
        }


        blit(grass, buffer, 0, 0, 0, 0, 800, 600);
        masked_blit(snake, buffer, image_x,image_y ,x,y, 21, 21);
        masked_blit(snake, buffer,28, 118, fruit_x,fruit_y,18,17);

        if (poison==true){
            masked_blit(snake,buffer,33,144,poison_x,poison_y,21,16);
        }



//-------------------------------------------------------------------------------------------
        // AI snake calculations along with "caught detection"
        int c_snake_x2 = c_snake_x+21;
        int c_snake_y2 = c_snake_y+21;


        if (x<c_snake_x){

            c_snake_x = c_snake_x-2.5;
            c_image_y = left;
        }
        if (x>c_snake_x){
            c_snake_x = c_snake_x+2.5;
            c_image_y = right;
        }
        if (y<c_snake_y){
            c_snake_y = c_snake_y-2.5;
            c_image_y = up;
        }
        if (y>c_snake_y){
            c_snake_y = c_snake_y +2.5;
            c_image_y = down;
        }

        c_image_x = c_image_x + 25;

        if(c_image_x>=75){c_image_x=0;}

        if (snake_x2<c_snake_x || (c_snake_x2) < x || snake_y2 < c_snake_y || (c_snake_y2) < y) {}
        else {

            done= true;
            caught = true;
        }

        masked_blit(snake, buffer, c_image_x,c_image_y, c_snake_x,c_snake_y,21,21);
        blit(buffer, screen, 0, 0, 0, 0, 800, 600);
        textprintf_ex(screen,font,10,10,makecol(255,100,200),-1,"Score : %d",score);
        textprintf_ex(screen,font,150,10,makecol(255,100,200),-1,"Rest time : %d",rest_time);
        rest(rest_time);

        // after so that the game screen doesnt flash after the game ends

    }

    //memory management and blitting the final score on the screen DEPENDING ON HOW YOU DIDED
    clear_bitmap(buffer);
    blit(buffer,screen,0,0,0,0,800,600);

    if (caught == true){
    textprintf_ex(screen,font,300,200,makecol(255,100,200),-1,"Computer is faster !!!");
        textprintf_ex(screen,font,300,300,makecol(255,100,200),-1,"Final Score : %d",score);
    }
    else {
    textprintf_ex(screen,font,300,200,makecol(255,100,200),-1,"You hit a wall!!");
    textprintf_ex(screen,font,300,300,makecol(255,100,200),-1,"Final Score : %d",score);
    }

    rest(2000);

    destroy_bitmap(buffer);
    destroy_bitmap(snake);
    destroy_bitmap(grass);
    destroy_bitmap(movement);
    destroy_sample(game_sound);

    return;

}

int main(){


    allegro_init(); //initialises Allegro for us
    install_timer();
    install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,"A");
    install_keyboard(); //install the keyboard handler
    set_color_depth(32); //sets the global color depth
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0); //creates the graphics mode

    //load all the tga images required for the game.
    BITMAP* buffer = create_bitmap(800, 600);
    clear_bitmap(buffer);

    BITMAP *intro = load_bitmap("intro.tga",NULL);
    BITMAP *story_1 = load_bitmap("story_1.tga",NULL);
    BITMAP *story_2 = load_bitmap("story_2.tga",NULL);
    BITMAP *zeus = load_bitmap ("zeus.tga",NULL);

    if(intro == 0) {allegro_message("Intro picture could not be loaded"); exit(1);}

    // some information for the marker
    /*textout_ex(buffer,font,"Please note ",100,140,makecol(0,0,255),-1);
    textout_ex(buffer,font,"The intro does not have music because the virtual machine was not outputting any sound.",100,160,makecol(255,0,0),-1);
    textout_ex(buffer,font,"The following functions should have sound attached to them : ",100,180,makecol(255,0,0),-1);
    textout_ex(buffer,font,"When the snake bites the FOOD source, not poison",100,200,makecol(255,0,0),-1);
    textout_ex(buffer,font,"When the game starts there is gameplay music",100,210,makecol(255,0,0),-1);
    textout_ex(buffer,font,"When the help option is selected from main screen",100,220,makecol(255,0,0),-1);
    textout_ex(buffer,font,"ALSO Help can be invoked by both : h or ctrl + h",100,250,makecol(255,0,0),-1);
    textout_ex(buffer,font,"Press any button to continue......",100,500,makecol(0,255,0),-1);


    blit(buffer,screen,0,0,0,0,800,600);


    while(!keypressed()){}

    clear_keybuf();
    clear_bitmap(buffer);

    textout_ex(buffer,font,"Sosha Games Presents.....",300,300,makecol(255,100,200),-1);
    blit(buffer,screen,0,0,0,0,800,600);

    rest(2500);

    clear_bitmap(buffer);

    for (int i=800;i>=0;i--){


        blit(story_1,buffer,0,0,0,i,800,600);
        blit(buffer,screen,0,0,0,0,800,600);

    }
    rest(15000);

    blit(zeus,buffer,0,0,0,0,800,600);
    blit(buffer,screen,0,0,0,0,800,600);
    rest(3000);

    for (int i=800;i>=0;i--){
        blit(story_2,buffer,0,0,0,i,800,600);
        blit(buffer,screen,0,0,0,0,800,600);

    }
    rest(15000);
    destroy_bitmap(story_1);
    destroy_bitmap(story_2);
    destroy_bitmap(zeus);

    clear_bitmap(buffer);

    int rest_time = 100;

    for (int i=0;i<40;i++){


        int temp_x=50+rand()%700;
        int temp_y = 50+rand()%500;

        int first = 5 +rand()%244;
        int second = 5 +rand()%244;
        int third = 5 +rand()%244;

        if (i==20){
            rest_time=200;
        }
        else if(i==30){
            rest_time=300;
        }
        else if(i==35){
            rest_time=400;
        }

        textout_ex(buffer,font,"Godly Snakes",temp_x,temp_y,makecol(first,second,third),-1);
        blit(buffer,screen,0,0,0,0,800,600);

        //textprintf_ex(screen,font,temp_x,temp_y,makecol(first,second,third),-1,"Godly Snakes");
        rest(rest_time);

        clear_bitmap(buffer);

        //textprintf_ex(screen,font,temp_x,temp_y,makecol(first,second,third),-1,"                ");

    }
    //final print to the screen
    textout_ex(buffer,font,"Godly Snakes: Green Lightning",300,300,makecol(255,0,0),-1);
    blit(buffer,screen,0,0,0,0,800,600);

    rest(3500);

    //menu options. Pressing these options will do difernent things.(*/
    bool done = false;

    while (done==false){
        blit(intro, buffer, 100, 300, 0, 0, 800, 600);
        blit(buffer, screen, 0, 0, 0, 0, 800, 600);

        while(!keypressed()){}

        if (key[KEY_P]){
            start_game();
            rest(222);
            clear_keybuf();
        }
        if ((key[KEY_LCONTROL] && key[KEY_H]) || (key[KEY_RCONTROL] && key[KEY_H]) || (key[KEY_H])){
            show_help();
            rest(222);
            clear_keybuf();
        }
        if (key[KEY_C]){
            show_credit();
            rest(222);
            clear_keybuf();
        }
        if(key[KEY_ESC]){
            done=true;
        }

    }

    //memory management
    destroy_bitmap(intro);
    destroy_bitmap(buffer);

    return 0;

}

END_OF_MAIN()
