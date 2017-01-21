#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>
#include<bits/stdc++.h>

using namespace std;

float block_size_x = 1;
float block_size_y = 1;
float block_size_z = 1;

float block_horizontal_gap = 0.2;
float block_vertical_gap = 0.2;

float blocks_z = 0;

vector<float> blocks_x;
vector<float> blocks_y;

int num_blocks = 0;

float player_x = 0;
float player_y = 0;
float player_z = 0;

float player_size_x = 1;
float player_size_y = 1;
float player_size_z = 1;

float player_accel_x = 0;

float ball_x = 0;
float ball_y = 0;
float ball_z = 0;

float ball_radius = 0.5;

float ball_accel_x = 0.001;
float ball_accel_y = 0.001;

float scene_left_x = -9.5;
float scene_right_x = 9.5;
float scene_top_y = 9.5;
float scene_bottom_y = -9.5;

bool game_running = true;

vector<float> power_up_x;
vector<float> power_up_y;

vector<float> power_accel_x;
vector<float> power_accel_y;

vector<int> power_up_type;

float power_up_z = 0;

float power_up_size_x = 1;
float power_up_size_y = 1;
float power_up_size_z = 1;

int num_power_ups = 0;

int score = 0;

void Write(char *string)
{
    while(*string)
    {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *string++);
    }
}

int generate_random(int min_n, int max_n)
{
    return min_n + ((rand() % (max_n - min_n))+1);
}

void Add_PowerUp(float p_x, float p_y,float a_x,float a_y, int ptype)
{
    power_up_x.push_back(p_x);
    power_up_y.push_back(p_y);
    power_accel_x.push_back(a_x);
    power_accel_y.push_back(a_y);
    power_up_type.push_back(ptype);
    num_power_ups++;
}

void Clear_PowerUps()
{
    power_up_x.clear();
    power_up_y.clear();
    power_accel_x.clear();
    power_accel_y.clear();
    power_up_type.clear();
    num_power_ups = 0;
}

void Remove_PowerUp(int ID)
{
    power_up_x.erase(power_up_x.begin() + ID);
    power_up_y.erase(power_up_y.begin() + ID);
    power_accel_x.erase(power_accel_x.begin() + ID);
    power_accel_y.erase(power_accel_y.begin() + ID);
    power_up_type.erase(power_up_type.begin() + ID);
    num_power_ups--;
}

int Collision_With_PowerUps()
{
    for (int I = num_power_ups - 1; I >= 0; I--)
    {

        if (((power_up_x[I] + power_up_size_x/2) >= (player_x - player_size_x/2)) && ((power_up_x[I] - power_up_size_x/2) <= (player_x+player_size_x/2)))
        {
            if (((power_up_y[I] + power_up_size_y/2) >= (player_y-player_size_y/2)) && ((power_up_y[I] - power_up_size_y/2) <= (player_y+player_size_y/2)))
            {
                return I;
            }
        }

    }
    return -1;
}


void Remove_Block(int ID)
{
    blocks_x.erase(blocks_x.begin() + ID);
    blocks_y.erase(blocks_y.begin() + ID);
    num_blocks--;
}

void Process_PowerUp(int Power_Up_Type)
{
    if (Power_Up_Type == 1)
    {
        if (player_size_x < 6)
        {
            player_size_x++;
        }
    }
    else if (Power_Up_Type == 0)
    {
        if (num_blocks == 0)
            return;

        int num_delete = 5;
        if (num_blocks < num_delete)
            num_delete = num_blocks;

        while (num_delete > 0)
        {
            if (num_blocks == 0)
                break;

            int rand_id = generate_random(1,num_blocks) - 1;
            Remove_Block(rand_id);
            num_delete--;

            score++;
        }
    }
    else if(Power_Up_Type==2)
    {
        ball_radius+=.1;
    }
    else if(Power_Up_Type==3)
    {
        game_running=false;
        return;
    }
}

void Update_PowerUps()
{
    for (int I = num_power_ups - 1; I >= 0; I--)
    {
        float c_x = power_up_x[I];
        float c_y = power_up_y[I];

        c_x = c_x + power_accel_x[I];
        c_y = c_y + power_accel_y[I];

        power_accel_y[I] = power_accel_y[I] - 0.00001;

        if (c_y < scene_bottom_y)
        {
            Remove_PowerUp(I);
        }
        else
        {
            power_up_x[I] = c_x;
            power_up_y[I] = c_y;
        }
    }

    int Collision_Result = Collision_With_PowerUps();
    if (Collision_Result > -1)
    {
        int pType = power_up_type[Collision_Result];
        Remove_PowerUp(Collision_Result);
        Process_PowerUp(pType);
    }
}

void Add_Block(float b_x, float b_y)
{
    blocks_x.push_back(b_x);
    blocks_y.push_back(b_y);
    num_blocks++;
}



void Clear_Blocks()
{
    blocks_x.clear();
    blocks_y.clear();
    num_blocks = 0;
}

void Reset_Parameters()
{
    player_accel_x = 0;
    player_x = 0;
    player_size_x = 1;
    score = 0;
}

void Create_Level(int Level)
{
    if (Level == 0)
    {
        Clear_Blocks();
        Clear_PowerUps();
        Reset_Parameters();
        for (int I = 0; I < 15; I++)
        {
            for (int J = 0; J < 5; J++)
            {
                float b_x = (I*block_size_x) + I*block_vertical_gap;
                float b_y = (J*block_size_y) + J*block_horizontal_gap;
                float c_x = (15.0/2.0)*block_size_x + (15.0/2.0)*block_vertical_gap - 0.5;
                float c_y = (5.0/2.0)*block_size_y + (5.0/2.0)*block_horizontal_gap - 5;
                Add_Block(b_x - c_x, b_y - c_y);
            }
        }
    }
}

int Collide_With_Player()
{
    if (((player_x + player_size_x/2) >= (ball_x-ball_radius)) && ((player_x - player_size_x/2) <= (ball_x+ball_radius)))
    {
        if (((player_y + player_size_y/2) >= (ball_y-ball_radius)) && ((player_y - player_size_y/2) <= (ball_y+ball_radius)))
        {
            if ((ball_x > (player_x+player_size_x/2)) && ((ball_y+ball_radius >= (player_y - player_size_y/2)) && (ball_y-ball_radius <= (player_y + player_size_y/2))))
            {
                return 2;
            }
            else if ((ball_x < (player_x-player_size_x/2)) && ((ball_y+ball_radius >= (player_y - player_size_y/2)) && (ball_y-ball_radius <= (player_y + player_size_y/2))))
            {
                return 2;
            }
            else
            {
                return 1;
            }
        }
    }

    return 0;
}

int Collide_With_Blocks()
{
    for (int I = num_blocks - 1; I >= 0; I--)
    {

        if (((blocks_x[I] + block_size_x/2) >= (ball_x-ball_radius)) && ((blocks_x[I] - block_size_x/2) <= (ball_x+ball_radius)))
        {
            if (((blocks_y[I] + block_size_y/2) >= (ball_y-ball_radius)) && ((blocks_y[I] - block_size_y/2) <= (ball_y+ball_radius)))
            {
                return I;
            }
        }

    }

    return -1;
}

void Create_Random_PowerUp(float px, float py)
{
    int rand_prob = generate_random(1,10);
    float r = (float)rand_prob/10.0;

    int rand_force = generate_random(1,10) - 5;
    float rand_x_force = (float)rand_force / 2000.0;

    rand_force = generate_random(1,5);
    float rand_y_force = -(float)rand_force / 2000.0;

    int rand_power_up_type;//
    //int rand_power_up_type= generate_random(0,1);

    rand_power_up_type=1;

    if (r <= 0.5)
    {
        Add_PowerUp(px,py,rand_x_force, rand_y_force,rand_power_up_type);
    }
}

void Update_Ball()
{
    float new_x = ball_x + ball_accel_x;
    float new_y = ball_y + ball_accel_y;

    /////////////////////////////////////////////////////////////////////////////
    if (new_x > scene_right_x)
    {
        new_x = scene_right_x;
        ball_accel_x = -ball_accel_x;

        int random_force = generate_random(1,4);
        if (ball_accel_x >= 0)
            ball_accel_x = ((float)(random_force)/1000.0);
        else
            ball_accel_x = -((float)(random_force)/1000.0);
    }
    else if (new_x < scene_left_x)
    {
        new_x = scene_left_x;
        ball_accel_x = -ball_accel_x;

        int random_force = generate_random(1,4);
        if (ball_accel_x >= 0)
            ball_accel_x = ((float)(random_force)/1000.0);
        else
            ball_accel_x = -((float)(random_force)/1000.0);
    }

    if (new_y > scene_top_y)
    {
        new_y = scene_top_y;
        ball_accel_y = -ball_accel_y;

        int random_force = generate_random(1,4);
        if (ball_accel_y >= 0)
            ball_accel_y = ((float)(random_force)/1000.0);
        else
            ball_accel_y = -((float)(random_force)/1000.0);
    }
    else if (new_y < scene_bottom_y)
    {
        /*
        new_y = scene_bottom_y;
        ball_accel_y = -ball_accel_y;

        int random_force = generate_random(1,4);
        if (ball_accel_y >= 0)
            ball_accel_y = ((float)(random_force)/1000.0);
        else
            ball_accel_y = -((float)(random_force)/1000.0);
        */
        game_running = false;
        return;
    }
    //////////////////////////////////////////////////////////////////////////////
    int Collision_Result = Collide_With_Player();
    if (Collision_Result != 0)
    {
        if (Collision_Result == 1)
        {
            // printf("Collision 1\n");
            ball_accel_y = -ball_accel_y;

            int random_force = generate_random(1,4);
            if (ball_accel_y >= 0)
                ball_accel_y = ((float)(random_force)/1000.0);
            else
                ball_accel_y = -((float)(random_force)/1000.0);

            new_y = ball_y + ball_accel_y;
        }
        else if (Collision_Result == 2)
        {
            //printf("Collision 2\n");
            ball_accel_x = -ball_accel_x;

            int random_force = generate_random(1,4);
            if (ball_accel_x >= 0)
                ball_accel_x = ((float)(random_force)/1000.0);
            else
                ball_accel_x = -((float)(random_force)/1000.0);

            new_x = ball_x + ball_accel_x;
        }
    }
    //////////////////////////////////////////////////////////////////////////////
    Collision_Result = Collide_With_Blocks();
    if (Collision_Result > -1)
    {
        ball_accel_y = -ball_accel_y;
        new_y = ball_y + ball_accel_y;

        float b_x = blocks_x[Collision_Result];
        float b_y = blocks_y[Collision_Result];

        Remove_Block(Collision_Result);
        score = score + 1;
        Create_Random_PowerUp(b_x,b_y);
    }

    /////////////////////////////////////////////////////////////////////////////
    ball_x = new_x;
    ball_y = new_y;
}

void Draw_Ball()
{
    glPushMatrix();

    glTranslatef(ball_x,ball_y,ball_z);
    glScalef(1,1,1);

    glutSolidSphere(ball_radius,20,20);

    glPopMatrix();
}

void Draw_Blocks()
{
    for (int I = num_blocks-1; I>= 0; I--)
    {

        glPushMatrix();

        glTranslatef(blocks_x[I], blocks_y[I], blocks_z);
        glScalef(block_size_x, block_size_y,block_size_z);

        glutSolidCube(1);

        glPopMatrix();

    }
}

void Draw_PowerUps()
{
    for (int I = num_power_ups-1; I>= 0; I--)
    {
        glColor3f(0,1,1);
        glPushMatrix();

        glTranslatef(power_up_x[I], power_up_y[I], power_up_z);
        glScalef(power_up_size_x, power_up_size_y,power_up_size_z);

        glutSolidCube(1);

        glPopMatrix();
        glColor3f(1,0,0);
    }
}


void Update_Player()
{
    player_accel_x = player_accel_x * 0.99;

    float new_x = player_x + player_accel_x;

    if (new_x < scene_left_x)
        new_x = scene_left_x;

    if (new_x > scene_right_x)
        new_x = scene_right_x;

    player_x = new_x;
}

void Draw_Player()
{
    glPushMatrix();

    glTranslatef(player_x, player_y, player_z);
    glScalef(player_size_x, player_size_y,player_size_z);

    glutSolidCube(1);

    glPopMatrix();
}


static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluOrtho2D(-10,10,-10,10);
    gluPerspective(60,ar,0.001,1000);
    //glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
}

static void display(void)
{
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    const double a = t*90.0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,0,0);

    ///////////////////////////////////////////////////////////////
    if (game_running == true)
    {
        glRasterPos2f(-10,-5);
        char score_str[50];
        sprintf(score_str, "Your Score : %d", score);
        Write(score_str);
    }
    else
    {
        glRasterPos2f(-5,0);
        char gameover_str[50];
        sprintf(gameover_str, "GAME OVER! Your Score : %d", score);
        Write(gameover_str);
    }
    ///////////////////////////////////////////////////////////////

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0,0,15,0,0,0,0,1,0);

    //////////////////////////////////////////////////////////////////////////
    if (game_running == true) Update_Player();
    if (game_running == true) Update_PowerUps();
    Draw_Blocks();
    Draw_Player();
    Draw_Ball();
    Draw_PowerUps();
    if (game_running == true) Update_Ball();

    glutSwapBuffers();
}


static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27 :
    case 'q':
        exit(0);
        break;
    case 'a':
        player_accel_x = player_accel_x - 0.01;
        if (player_accel_x < -0.2)
            player_accel_x = -0.2;
        break;
    case 's':
        player_accel_x = player_accel_x + 0.01;
        if (player_accel_x > 0.2)
            player_accel_x = 0.2;
        break;
    }

    glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 0.50f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

int main(int argc, char *argv[])
{
    //cout<<"shuvro"<<endl;
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Abit's DX BALL");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    glClearColor(1,1,1,1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    ///////////////////////////////////////////////////////////////////////////////////////////////
    Create_Level(0);
    //////////////////////////////////////////////////////////////////////////////////////////////////
    player_y = -8;
    player_size_x = 4;
    //////////////////////////////////////////////////////////////////////////////////////////////////

    glutMainLoop();

    return EXIT_SUCCESS;
}
