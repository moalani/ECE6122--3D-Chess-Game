/*
 Author: Mohammed Alani
 Class: ECE6122
 Last Date Modified: 11/24/2019
 Description: this file serves as my submission for hmk6. It is a 3D chess board with some functionality, such as randomly moving a knight, or a pawn, changing lighting, and allowing enhanced mode with better looking chess pieces.
 
 Note that the center of the chess board is point (0,0,0) and each section is either in the negative (-4 to 0) or positive (0 to 4)
 
 Enhanced Session has been implemented, the code might take <3 seconds to rotate or move due to the graphics involved.

 Please note that
 ****** the functionloadObj function and .obj files were sourced from MIT open source code from github page below: https://github.com/bourgeoisor/chess3d
 
 */
// for MAC only
/*
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <OpenGl/glu.h>
#include <GLUT/glut.h>
*/

// standard libraries loaded
#include <sys/time.h>
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <math.h>
#include <fstream>
#include <chrono>
#include <sstream>
#include <string>



#include <GL/gl.h> // Include the GLEW header file
#include <GL/glut.h> // Include the GLUT header file
#include <GL/glu.h>
 
using namespace std;
#define ESC 27


// defining lighting, and source
GLfloat light0_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat light1_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat light1_diffuse[] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat light1_specular[] = { 0.3, 0.3, 0.3, 1.0 };
GLfloat light1_position[] = { -9, -9, 8, 0.0 };

GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_shininess[] = { 50.0 };

// below are global variables defined that will be later be used by the pieces spawning or changing setting or mode of the chess board.

bool GLLight0_flag = true;
bool GLLight1_flag = true;

float eye_z = 10;
float x = 4, y = -5.0; // initially 5 units south of origin
float deltaMove = 0.0; // initially camera doesn't move

// Camera direction and position arrays

float lx = 4, ly = 4; // camera points initially along y-axis
float angle = 0.0; // angle of rotation for the camera direction
float deltaAngle = 0.0; // additional angle change when dragging


int offset = 75;
int windowWidth = 600;
int windowHeight = 600;
int rot = 0;
int rot_prev = 0;
int x_box = 75, y_box = 75;
bool isBlack = true;

// defining the size of speheres that will be used for pawns later

float sphereDiameter = 0.75;
float sphereRadius = sphereDiameter/2;

int randKnight = rand() % 4;
int randPawn = rand() % 16;


/// Defining team A pieces ///
float x_off = -4.0;
float y_off = -4.0;

float teamA_r1_y = 1.5 + x_off;
float teamA_r1_x = 0.5 + y_off;

float teamA_r2_y = 1.5+ x_off;
float teamA_r2_x = 1.5 + y_off;

float teamA_r3_y = 1.5+ x_off;
float teamA_r3_x = 2.5 + y_off;

float teamA_r4_y = 1.5+ x_off;
float teamA_r4_x = 3.5 + y_off;

float teamA_r5_y = 1.5+ x_off;
float teamA_r5_x = 4.5 + y_off;

float teamA_r6_y = 1.5+ x_off;
float teamA_r6_x = 5.5 + y_off;

float teamA_r7_y = 1.5+ x_off;
float teamA_r7_x = 6.5 + y_off;

float teamA_r8_y = 1.5 + x_off;
float teamA_r8_x = 7.5 + y_off;

float teamA_rook_1_x = 0.5+ x_off;
float teamA_rook_1_y = 0.5 + y_off;

float teamA_rook_2_x = 7.5 + x_off;
float teamA_rook_2_y = 0.5 + y_off;

float teamA_knight_1_x = 1.5 + x_off;
float teamA_knight_1_y = /*1*/ 0.5 + y_off;

float teamA_knight_2_x = 6.5 + x_off;
float teamA_knight_2_y = 0.5 + y_off;

float teamA_bishop_1_x = 2.5 + x_off;
float teamA_bishop_1_y = /*1*/0.5 + y_off;

float teamA_bishop_2_x = 5.5 + x_off;
float teamA_bishop_2_y = 0.5 + y_off;

float teamA_queen_1_x = 3.5 + x_off;
float teamA_queen_1_y = /*1*/0.5 + y_off;

float teamA_king_2_x = 4.5 + x_off;
float teamA_king_2_y = 0.5 + y_off;

/// Defining team B pieces ///

float teamB_r1_y = 6.5 + x_off;
float teamB_r1_x = 0.5 + y_off;

float teamB_r2_y = 6.5 + x_off;
float teamB_r2_x = 1.5 + y_off;

float teamB_r3_y = 6.5 + x_off;
float teamB_r3_x = 2.5 + y_off;

float teamB_r4_y = 6.5 + x_off;
float teamB_r4_x = 3.5 + y_off;

float teamB_r5_y = 6.5 + x_off;
float teamB_r5_x = 4.5 + y_off;

float teamB_r6_y = 6.5 + x_off;
float teamB_r6_x = 5.5 + y_off;

float teamB_r7_y = 6.5 + x_off;
float teamB_r7_x = 6.5 + y_off;

float teamB_r8_y = 6.5 + x_off;
float teamB_r8_x = 7.5 + y_off;

float teamB_rook_1_x = 0.5 + x_off;
float teamB_rook_1_y = 7.5 + y_off;

float teamB_rook_2_x = 7.5 + x_off;
float teamB_rook_2_y = 7.5 + y_off;

float teamB_knight_1_x = 1.5 + x_off;
float teamB_knight_1_y = 7.5 + y_off;

float teamB_knight_2_x = 6.5 + x_off;
float teamB_knight_2_y = 7.5 + y_off;

float teamB_bishop_1_x = 2.5 + x_off;
float teamB_bishop_1_y = 7.5+ y_off;

float teamB_bishop_2_x = 5.5 + x_off;
float teamB_bishop_2_y = 7.5 + y_off;

float teamB_queen_1_x = 3.5 + x_off;
float teamB_queen_1_y = 7.5 + y_off;

float teamB_king_2_x = 4.5 + x_off;
float teamB_king_2_y = 7.5 + y_off;


bool enhancedMode = false;
GLfloat position[] = { 0.0, 0.0, 0.5, 0.5 };
/*
 
 function: loadObj will load object file and take the team number as an argument in order to render
 the enhanced graphics.
 This function returns GLuint object as an output
 
 */


GLuint loadObj(std::string filename, int teamNum) {
    std::ifstream inputfile(filename);
    
    std::vector<std::vector<float>> vertices, normals, textures;
    std::vector<unsigned int> vertex_indices, normal_indices, texture_indices;
    
    std::string str;
    while (std::getline(inputfile, str)) {
        if (str.size() > 2) {
            std::istringstream iss(str);
            std::string token;
            float vec[3];
            
            // Reads a vertex.
            if (str[0] == 'v' && str[1] == ' ') {
                std::getline(iss, token, ' ');
                
                for (int i = 0; i < 3; ++i) {
                    std::getline(iss, token, ' ');
                    vec[i] = std::stof(token);
                    //cout << vec[i] << endl;
                }
                
                vertices.push_back({vec[0], vec[1], vec[2]});
            }
            // Reads a normal.
            else if (str[0] == 'v' && str[1] == 'n') {
                getline(iss, token, ' ');
                
                for (int i = 0; i < 3; ++i) {
                    getline(iss, token, ' ');
                    vec[i] = std::stof(token);
                }
                
                normals.push_back({vec[0], vec[1], vec[2]});
            }
            // Reads a texture.
            else if (str[0] == 'v' && str[1] == 't') {
                getline(iss, token, ' ');
                
                for (int i = 0; i < 2; ++i) {
                    getline(iss, token, ' ');
                    vec[i] = std::stof(token);
                }
                
                textures.push_back({vec[0], vec[1]});
            }
            // Reads a vertex/texture/normal index list.
            else if (str[0] == 'f' && str[1] == ' ') {
                getline(iss, token, ' ');
                
                for (int i = 0; i < 3; ++i) {
                    getline(iss, token, ' ');
                    size_t index;
                    
                    if ((index = token.find("//")) != std::string::npos) {
                        std::string a = token.substr(0, index);
                        std::string b = token.substr(index + 2, token.size() - index);
                        
                        vertex_indices.push_back(std::stoi(a) - 1);
                        normal_indices.push_back(std::stoi(b) - 1);
                    }
                    else {
                        index = token.find("/");
                        size_t index2 = token.find("/", index + 1);
                        std::string a = token.substr(0, index);
                        std::string b = token.substr(index + 1, index2 - index - 1);
                        std::string c = token.substr(index2 + 1, token.size() - index2 + 1);
                        
                        
                        vertex_indices.push_back(std::stoi(a) - 1);
                        texture_indices.push_back(std::stoi(b) - 1);
                        normal_indices.push_back(std::stoi(c) - 1);
                    }
                }
            }
        }
    }
    
    // Initializes the object.
    GLuint object = glGenLists(1);
    glNewList(object, GL_COMPILE);
    glPushMatrix();
    {
        glPointSize(1.0);
        
        // Draws the triangles of the object.
        glBegin(GL_TRIANGLES);
        {
            if (teamNum == 0)
            {
                glColor3f(150.0/255.0, 75.0/255.0, 0.0/255.0);
                
            }
            
            else
            {
                glColor3f(140.0/255.0, 140.0/255.0, 135.0/255.0);
            }
            
            for (int i = 0; i < vertex_indices.size(); i+=3) {
                glNormal3f(normals[normal_indices[i]][0], normals[normal_indices[i]][1], normals[normal_indices[i]][2]);
                glVertex3f(vertices[vertex_indices[i]][0], vertices[vertex_indices[i]][1], vertices[vertex_indices[i]][2]);
                
                glNormal3f(normals[normal_indices[i+1]][0], normals[normal_indices[i+1]][1], normals[normal_indices[i+1]][2]);
                glVertex3f(vertices[vertex_indices[i+1]][0], vertices[vertex_indices[i+1]][1], vertices[vertex_indices[i+1]][2]);
                
                glNormal3f(normals[normal_indices[i+2]][0], normals[normal_indices[i+2]][1], normals[normal_indices[i+2]][2]);
                glVertex3f(vertices[vertex_indices[i+2]][0], vertices[vertex_indices[i+2]][1], vertices[vertex_indices[i+2]][2]);
            }
        }
        glEnd();
    }
    glPopMatrix();
    glEndList();
    
    return object;
}

/*
 
 function: validKnightMove will determine if the knight move is legal (i.e. not stepping on another piece), by looking at where all the pieces are located
 This function is boolean, so it returns true if the move is valid and false otherwise
 
 */

bool validKnightMove(float nextMove[2], float piecesLocation[32][2])
{
    for (int i = 0 ; i < 32; i++)
    {
        if (((nextMove[0] == piecesLocation[i][0]) && (nextMove[1] == piecesLocation[i][1])) || ((nextMove[0] < -3.5 || nextMove[1] < -3.5 || nextMove[0] > 3.5 || nextMove[1] > 3.5)))
        {
            return false;
        }

    }
    
    return true;
}

/*
 
 function: validPawnMove will determine if the pawn move is legal (i.e. not stepping on another piece), by looking at where all the pieces are located
 This function is boolean, so it returns true if the move is valid and false otherwise
 
 */

bool validPawnMove(float nextMove[2], float piecesLocation[32][2])
{
    for (int i = 0 ; i < 32; i++)
    {
        if (((nextMove[0] == piecesLocation[i][0]) && (nextMove[1] == piecesLocation[i][1])) || ((nextMove[0] < -3.5 || nextMove[1] < -3.5 || nextMove[0] > 3.5 || nextMove[1] > 3.5)))
        {
            return false;
        }
        
    }
    
    return true;
}

/*
 
 function: changeSize will load the window and change according to any change in the window size, and assist in re-spawning the grpahics when needed.
 This function is void
 
 */


void changeSize(int w, int h)
{
    float ratio = ((float)w) / ((float)h); // window aspect ratio
    glMatrixMode(GL_PROJECTION); // projection matrix is active
    glLoadIdentity(); // reset the projection
    gluPerspective(45.0, ratio, 0.1, 100.0); // perspective transformation
    glMatrixMode(GL_MODELVIEW); // return to modelview mode
    glViewport(0, 0, w, h); // set viewport (drawing area) to entire window
}


void myDisplay(void)
{

    // setting up the lighting paramaters by default
    glClearColor(0.0, 0.7, 1.0, 1.0); // sky color is light blue
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glShadeModel(GL_SMOOTH);
    
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess[0]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glEnable(GL_COLOR_MATERIAL);
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    
    
    // setting the camera angle according to the requirements

    gluLookAt(0 , -10, eye_z, 0 ,  0, 0,0, 0, 10);

    // setting the lighting flags by default, those will be referenced later in keyboard strokes
    if (GLLight0_flag == true)
    {
        glEnable(GL_LIGHT0);
    }
    
    if (GLLight0_flag == false)
    {
        glDisable(GL_LIGHT0);
    }
    
    if (GLLight1_flag == false)
    {
        glDisable(GL_LIGHT1);
    }
    
    if (GLLight1_flag == true)
    {
        glEnable(GL_LIGHT1);
    }

    
    // this for loop is for placing the pieces of the chess board. The for loop put them at alternating positions
    for (int i = -4; i < 4; i++)
    {
        for (int j = -4; j < 4; j++)
        {
            if ((i+j) % 2 == 0)
            {
                
                // black boxes in the board
                glColor3f(0/255.0, 0/255.0, 0.0/255.0);
                glPushMatrix();
                glRotatef(rot, 0.0f, 0.0f, 1.0f);
                glBegin(GL_POLYGON);
                glVertex3f(i, j, 0.0);
                glVertex3f(i, j+1, 0.0);
                glVertex3f(i+1, j+1, 0.0);
                glVertex3f(i+1, j, 0.0);
                glEnd();
                glPopMatrix();
            }
            else
            {
                // white boxes in the board
                glColor3f(255.0/255.0, 255.0/255.0, 255.0/255.0);
                glPushMatrix();
                glRotatef(rot, 0.0f, 0.0f, 1.0f);
                glBegin(GL_POLYGON);
                glVertex3f(i, j, 0.0);
                glVertex3f(i, j+1, 0.0);
                glVertex3f(i+1, j+1, 0.0);
                glVertex3f(i+1, j, 0.0);
                glEnd();
                glPopMatrix();
            }
        }
        // loading the enhanced objects
        static GLuint enhancedPawn0 = loadObj("pawn.obj",0);
        static GLuint enhancedPawn1 = loadObj("pawn.obj",1);
        static GLuint enhancedRook0 = loadObj("rook.obj",0);
        static GLuint enhancedRook1 = loadObj("rook.obj",1);
        static GLuint enhancedBishop0 = loadObj("bishop.obj",0);
        static GLuint enhancedBishop1 = loadObj("bishop.obj",1);
        static GLuint enhancedKnight0 = loadObj("knight.obj",0);
        static GLuint enhancedKnight1 = loadObj("knight.obj",1);
        static GLuint enhancedQueen0 = loadObj("queen.obj",0);
        static GLuint enhancedQueen1 = loadObj("queen.obj",1);
        static GLuint enhancedKing0 = loadObj("king.obj",0);
        static GLuint enhancedKing1 = loadObj("king.obj",1);
        
        // determining if we are in enhanced mode or not
        
        if (enhancedMode == true)
        {
            
            // ***** LOADING TEAM A PIECES ***** //
            
            glPushMatrix();
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamA_r1_x,teamA_r1_y,0);
            glRotatef(90, 1.0, 0.0, 0.0);
            glRotatef(90, 0.0, 1.0, 0.0);
            glScalef(0.008,0.008,0.008);
            glCallList(enhancedPawn0);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(150.0/255.0, 75.0/255.0, 0.0/255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamA_r2_x,teamA_r2_y,0);
            glRotatef(90, 1.0, 0.0, 0.0);
            glRotatef(90, 0.0, 1.0, 0.0);
            glScalef(0.009,0.009,0.009);
            glCallList(enhancedPawn0);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(150.0/255.0, 75.0/255.0, 0.0/255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamA_r3_x,teamA_r3_y,0);
            glRotatef(90, 1.0, 0.0, 0.0);
            glRotatef(90, 0.0, 1.0, 0.0);
            glScalef(0.009,0.009,0.009);
            glCallList(enhancedPawn0);
            glPopMatrix();
            
            
            glPushMatrix();
            glColor3f(150.0/255.0, 75.0/255.0, 0.0/255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamA_r4_x,teamA_r4_y,0);
            glRotatef(90, 1.0, 0.0, 0.0);
            glRotatef(90, 0.0, 1.0, 0.0);
            glScalef(0.009,0.009,0.009);
            glCallList(enhancedPawn0);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(150.0/255.0, 75.0/255.0, 0.0/255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamA_r5_x,teamA_r5_y,0);
            glRotatef(90, 1.0, 0.0, 0.0);
            glRotatef(90, 0.0, 1.0, 0.0);
            glScalef(0.009,0.009,0.009);
            glCallList(enhancedPawn0);
            glPopMatrix();
            
            
            glPushMatrix();
            glColor3f(150.0/255.0, 75.0/255.0, 0.0/255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamA_r6_x,teamA_r6_y,0);
            glRotatef(90, 1.0, 0.0, 0.0);
            glRotatef(90, 0.0, 1.0, 0.0);
            glScalef(0.009,0.009,0.009);
            glCallList(enhancedPawn0);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(150.0/255.0, 75.0/255.0, 0.0/255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamA_r7_x,teamA_r7_y,0);
            glRotatef(90, 1.0, 0.0, 0.0);
            glRotatef(90, 0.0, 1.0, 0.0);
            glScalef(0.009,0.009,0.009);
            glCallList(enhancedPawn0);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(150.0/255.0, 75.0/255.0, 0.0/255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamA_r8_x,teamA_r8_y,0);
            glRotatef(90, 1.0, 0.0, 0.0);
            glRotatef(90, 0.0, 1.0, 0.0);
            glScalef(0.009,0.009,0.009);
            glCallList(enhancedPawn0);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(150.0/255.0, 75.0/255.0, 0.0/255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamA_rook_1_x,teamA_rook_1_y,0);//0.75/2);
            glRotatef(90, 1.0, 0.0, 0.0);
            glRotatef(90, 0.0, 1.0, 0.0);
            glScalef(0.009,0.009,0.009);
            glCallList(enhancedRook0);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(150.0/255.0, 75.0/255.0, 0.0/255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamA_rook_2_x,teamA_rook_2_y,0);//0.75/2);
            glRotatef(90, 1.0, 0.0, 0.0);
            glRotatef(90, 0.0, 1.0, 0.0);
            glScalef(0.009,0.009,0.009);
            glCallList(enhancedRook0);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(150.0/255.0, 75.0/255.0, 0.0/255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamA_knight_1_x,teamA_knight_1_y,0);//0.75/2);
            glRotatef(90, 1.0, 0.0, 0.0);
            glRotatef(90, 0.0, 1.0, 0.0);
            glScalef(0.009,0.009,0.009);
            glCallList(enhancedKnight0);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(150.0/255.0, 75.0/255.0, 0.0/255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamA_knight_2_x ,teamA_knight_2_y,0);//(0.75/2)  );
            glRotatef(90, 1.0, 0.0, 0.0);
            glRotatef(90, 0.0, 1.0, 0.0);
            glScalef(0.009,0.009,0.009);
            glCallList(enhancedKnight0);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(150.0/255.0, 75.0/255.0, 0.0/255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamA_bishop_1_x,teamA_bishop_1_y,0);
            glRotatef(90, 1.0, 0.0, 0.0);
            glRotatef(90, 0.0, 1.0, 0.0);
            glScalef(0.009,0.009,0.009);
            glCallList(enhancedBishop0);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(150.0/255.0, 75.0/255.0, 0.0/255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamA_bishop_2_x,teamA_bishop_2_y,0);
            glRotatef(90, 1.0, 0.0, 0.0);
            glRotatef(90, 0.0, 1.0, 0.0);
            glScalef(0.009,0.009,0.009);
            glCallList(enhancedBishop0);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(150.0/255.0, 75.0/255.0, 0.0/255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamA_queen_1_x,teamA_queen_1_y,0);
            glRotatef(90, 1.0, 0.0, 0.0);
            glRotatef(90, 0.0, 1.0, 0.0);
            glScalef(0.009,0.009,0.009);
            glCallList(enhancedQueen0);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(150.0/255.0, 75.0/255.0, 0.0/255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamA_king_2_x,teamA_king_2_y,0);
            glRotatef(90, 1.0, 0.0, 0.0);
            glRotatef(90, 0.0, 1.0, 0.0);
            glScalef(0.009,0.009,0.009);
            glCallList(enhancedKing0);
            glPopMatrix();
            
    /* ****************   TEAM B ********************* */
            
            // **** SETTING UP THE PIECES for TEAM B
            
            glPushMatrix();
            glColor3f(140.0/255.0, 140.0/255.0, 135.0/255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamB_r1_x,teamB_r1_y,0);
            glRotatef(90, 1.0, 0.0, 0.0);
            glRotatef(90, 0.0, 1.0, 0.0);
            glScalef(0.009,0.009,0.009);
            glCallList(enhancedPawn1);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(140.0/255.0, 140.0/255.0, 135.0/255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamB_r2_x,teamB_r2_y,0);
            glRotatef(90, 1.0, 0.0, 0.0);
            glRotatef(90, 0.0, 1.0, 0.0);
            glScalef(0.009,0.009,0.009);

            //glutSolidSphere(0.75/2,50,50);
            glCallList(enhancedPawn1);

            glPopMatrix();
            
            glPushMatrix();
            glColor3f(140.0/255.0, 140.0/255.0, 135.0/255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamB_r3_x,teamB_r3_y,0);
            glRotatef(90, 1.0, 0.0, 0.0);
            glRotatef(90, 0.0, 1.0, 0.0);
            glScalef(0.009,0.009,0.009);
            glCallList(enhancedPawn1);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(140.0/255.0, 140.0/255.0, 135.0/255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamB_r4_x,teamB_r4_y,0);
            glRotatef(90, 1.0, 0.0, 0.0);
            glRotatef(90, 0.0, 1.0, 0.0);
            glScalef(0.009,0.009,0.009);
            glCallList(enhancedPawn1);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(140.0/255.0, 140.0/255.0, 135.0/255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamB_r5_x,teamB_r5_y,0);
            glRotatef(90, 1.0, 0.0, 0.0);
            glRotatef(90, 0.0, 1.0, 0.0);
            glScalef(0.009,0.009,0.009);
            glCallList(enhancedPawn1);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(140.0/255.0, 140.0/255.0, 135.0/255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamB_r6_x,teamB_r6_y,0);
            glRotatef(90, 1.0, 0.0, 0.0);
            glRotatef(90, 0.0, 1.0, 0.0);
            glScalef(0.009,0.009,0.009);
            glCallList(enhancedPawn1);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(140.0/255.0, 140.0/255.0, 135.0/255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamB_r7_x,teamB_r7_y,0);
            glRotatef(90, 1.0, 0.0, 0.0);
            glRotatef(90, 0.0, 1.0, 0.0);
            glScalef(0.009,0.009,0.009);
            glCallList(enhancedPawn1);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(140.0/255.0, 140.0/255.0, 135.0/255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamB_r8_x,teamB_r8_y,0);
            glRotatef(90, 1.0, 0.0, 0.0);
            glRotatef(90, 0.0, 1.0, 0.0);
            glScalef(0.009,0.009,0.009);
            glCallList(enhancedPawn1);
            glPopMatrix();
            
            
            glPushMatrix();
            glColor3f(140.0/255.0, 140.0/255.0, 135.0/255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamB_rook_1_x,teamB_rook_1_y,0);//0.75/2);
            glRotatef(90, 1.0, 0.0, 0.0);
            glRotatef(90, 0.0, 1.0, 0.0);
            glScalef(0.009,0.009,0.009);
            glCallList(enhancedRook1);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(140.0/255.0, 140.0/255.0, 135.0/255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamB_rook_2_x,teamB_rook_2_y,0);//0.75/2);
            glRotatef(90, 1.0, 0.0, 0.0);
            glRotatef(90, 0.0, 1.0, 0.0);
            glScalef(0.009,0.009,0.009);
            glCallList(enhancedRook1);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(140.0/255.0, 140.0/255.0, 135.0/255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamB_knight_1_x,teamB_knight_1_y,0);//0.75/2);
            glRotatef(90, 1.0, 0.0, 0.0);
            glRotatef(90, 0.0, 1.0, 0.0);
            glScalef(0.009,0.009,0.009);
            glCallList(enhancedKnight1);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(140.0/255.0, 140.0/255.0, 135.0/255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamB_knight_2_x,teamB_knight_2_y,0);//0.75/2);
            glRotatef(90, 1.0, 0.0, 0.0);
            glRotatef(90, 0.0, 1.0, 0.0);
            glScalef(0.009,0.009,0.009);
            glCallList(enhancedKnight1);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(140.0/255.0, 140.0/255.0, 135.0/255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamB_bishop_1_x,teamB_bishop_1_y,0);
            glRotatef(90, 1.0, 0.0, 0.0);
            glRotatef(90, 0.0, 1.0, 0.0);
            glScalef(0.009,0.009,0.009);
            glCallList(enhancedBishop1);

            glPopMatrix();
            
            glPushMatrix();
            glColor3f(140.0/255.0, 140.0/255.0, 135.0/255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamB_bishop_2_x,teamB_bishop_2_y,0);
            glRotatef(90, 1.0, 0.0, 0.0);
            glRotatef(90, 0.0, 1.0, 0.0);
            glScalef(0.009,0.009,0.009);
            glCallList(enhancedBishop1);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(140.0/255.0, 140.0/255.0, 135.0/255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamB_queen_1_x,teamB_queen_1_y,0);//0.3);
            glRotatef(90, 1.0, 0.0, 0.0);
            glRotatef(90, 0.0, 1.0, 0.0);
            glScalef(0.009,0.009,0.009);
            glCallList(enhancedQueen1);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(140.0/255.0, 140.0/255.0, 135.0/255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamB_king_2_x,teamB_king_2_y,0);//0.5);
            glRotatef(90, 1.0, 0.0, 0.0);
            glRotatef(90, 0.0, 1.0, 0.0);
            glScalef(0.009,0.009,0.009);
            glCallList(enhancedKing1);
            glPopMatrix();

    }
        // If we are not in the enhanced mode session, then we put the normal pieces.
        else
        {
            glPushMatrix();
            glColor3f(150.0 / 255.0, 75.0 / 255.0, 0.0 / 255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamA_r1_x, teamA_r1_y, sphereRadius);
            glutSolidSphere(0.75 / 2, 50, 50);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(150.0 / 255.0, 75.0 / 255.0, 0.0 / 255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamA_r2_x, teamA_r2_y, sphereRadius);
            glutSolidSphere(0.75 / 2, 50, 50);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(150.0 / 255.0, 75.0 / 255.0, 0.0 / 255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamA_r3_x, teamA_r3_y, sphereRadius);
            glutSolidSphere(0.75 / 2, 50, 50);
            glScalef(5, 5, 5);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(150.0 / 255.0, 75.0 / 255.0, 0.0 / 255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamA_r4_x, teamA_r4_y, sphereRadius);
            glutSolidSphere(0.75 / 2, 50, 50);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(150.0 / 255.0, 75.0 / 255.0, 0.0 / 255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamA_r5_x, teamA_r5_y, sphereRadius);
            glutSolidSphere(0.75 / 2, 50, 50);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(150.0 / 255.0, 75.0 / 255.0, 0.0 / 255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamA_r6_x, teamA_r6_y, sphereRadius);
            glutSolidSphere(0.75 / 2, 50, 50);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(150.0 / 255.0, 75.0 / 255.0, 0.0 / 255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamA_r7_x, teamA_r7_y, sphereRadius);
            glutSolidSphere(0.75 / 2, 50, 50);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(150.0 / 255.0, 75.0 / 255.0, 0.0 / 255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamA_r8_x, teamA_r8_y, sphereRadius);
            glutSolidSphere(0.75 / 2, 50, 50);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(150.0 / 255.0, 75.0 / 255.0, 0.0 / 255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamA_rook_1_x, teamA_rook_1_y, 0.75 / 2);
            glScalef(0.75, 0.75, 0.75);
            glutSolidCube(0.75);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(150.0 / 255.0, 75.0 / 255.0, 0.0 / 255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamA_rook_2_x, teamA_rook_2_y, 0.75 / 2);
            glScalef(0.75, 0.75, 0.75);
            glutSolidCube(0.75);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(150.0 / 255.0, 75.0 / 255.0, 0.0 / 255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamA_knight_1_x, teamA_knight_1_y, 0.75 / 2);
            glRotatef(90, 1.0, 0.0, 0.0);
            glRotatef(90, 0.0, 1.0, 0.0);
            glScalef(0.3, 0.5, 0.4);
            glutSolidTeapot(0.75);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(150.0 / 255.0, 75.0 / 255.0, 0.0 / 255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamA_knight_2_x, teamA_knight_2_y, 0.75 / 2);
            glRotatef(90, 1.0, 0.0, 0.0);
            glRotatef(90, 0.0, 1.0, 0.0);
            glScalef(0.3, 0.5, 0.4);
            glutSolidTeapot(0.75);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(150.0 / 255.0, 75.0 / 255.0, 0.0 / 255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamA_bishop_1_x, teamA_bishop_1_y, 0);
            glScalef(0.5, 0.5, 0.5);
            glutSolidCone(0.75, 1, 50, 50);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(150.0 / 255.0, 75.0 / 255.0, 0.0 / 255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamA_bishop_2_x, teamA_bishop_2_y, 0);
            glScalef(0.5, 0.5, 0.5);
            glutSolidCone(0.75, 1, 50, 50);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(150.0 / 255.0, 75.0 / 255.0, 0.0 / 255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamA_queen_1_x, teamA_queen_1_y, 0.3);
            glRotatef(90, 1.0, 0.0, 0.0);
            glRotatef(90, 0.0, 1.0, 0.0);
            glScalef(0.5, 0.5, 0.5);
            glutSolidTetrahedron();
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(150.0 / 255.0, 75.0 / 255.0, 0.0 / 255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamA_king_2_x, teamA_king_2_y, 0.5);
            glRotatef(45, 1.0, 0.0, 0.0);
            glRotatef(45, 0.0, 1.0, 0.0);
            glScalef(0.5, 0.5, 0.5);
            glutSolidOctahedron();
            glPopMatrix();
            
            /* ****************   TEAM B ********************* */
            
            glPushMatrix();
            glColor3f(140.0 / 255.0, 140.0 / 255.0, 135.0 / 255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamB_r1_x, teamB_r1_y, sphereRadius);
            glutSolidSphere(0.75 / 2, 50, 50);
            glScalef(5, 5, 5);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(140.0 / 255.0, 140.0 / 255.0, 135.0 / 255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamB_r2_x, teamB_r2_y, sphereRadius);
            glutSolidSphere(0.75 / 2, 50, 50);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(140.0 / 255.0, 140.0 / 255.0, 135.0 / 255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamB_r3_x, teamB_r3_y, sphereRadius);
            glutSolidSphere(0.75 / 2, 50, 50);
            glScalef(5, 5, 5);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(140.0 / 255.0, 140.0 / 255.0, 135.0 / 255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamB_r4_x, teamB_r4_y, sphereRadius);
            glutSolidSphere(0.75 / 2, 50, 50);
            glScalef(5, 5, 5);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(140.0 / 255.0, 140.0 / 255.0, 135.0 / 255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamB_r5_x, teamB_r5_y, sphereRadius);
            glutSolidSphere(0.75 / 2, 50, 50);
            glScalef(5, 5, 5);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(140.0 / 255.0, 140.0 / 255.0, 135.0 / 255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamB_r6_x, teamB_r6_y, sphereRadius);
            glutSolidSphere(0.75 / 2, 50, 50);
            glScalef(5, 5, 5);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(140.0 / 255.0, 140.0 / 255.0, 135.0 / 255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamB_r7_x, teamB_r7_y, sphereRadius);
            glutSolidSphere(0.75 / 2, 50, 50);
            glScalef(5, 5, 5);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(140.0 / 255.0, 140.0 / 255.0, 135.0 / 255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamB_r8_x, teamB_r8_y, sphereRadius);
            glutSolidSphere(0.75 / 2, 50, 50);
            glScalef(5, 5, 5);
            glPopMatrix();
            
            
            glPushMatrix();
            glColor3f(140.0 / 255.0, 140.0 / 255.0, 135.0 / 255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamB_rook_1_x, teamB_rook_1_y, 0.75 / 2);
            glScalef(0.75, 0.75, 0.75);
            glutSolidCube(0.75);
            //glScalef(5,5,5);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(140.0 / 255.0, 140.0 / 255.0, 135.0 / 255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamB_rook_2_x, teamB_rook_2_y, 0.75 / 2);
            glScalef(0.75, 0.75, 0.75);
            glutSolidCube(0.75);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(140.0 / 255.0, 140.0 / 255.0, 135.0 / 255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamB_knight_1_x, teamB_knight_1_y, 0.75 / 2);
            glRotatef(90, 1.0, 0.0, 0.0);
            glRotatef(90, 0.0, 1.0, 0.0);
            glScalef(0.3, 0.5, 0.4);
            glutSolidTeapot(0.75);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(140.0 / 255.0, 140.0 / 255.0, 135.0 / 255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamB_knight_2_x, teamB_knight_2_y, 0.75 / 2);
            glRotatef(90, 1.0, 0.0, 0.0);
            glRotatef(90, 0.0, 1.0, 0.0);
            glScalef(0.3, 0.5, 0.4);
            glutSolidTeapot(0.75);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(140.0 / 255.0, 140.0 / 255.0, 135.0 / 255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamB_bishop_1_x, teamB_bishop_1_y, 0);
            glScalef(0.5, 0.5, 0.5);
            glutSolidCone(0.75, 1, 50, 50);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(140.0 / 255.0, 140.0 / 255.0, 135.0 / 255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamB_bishop_2_x, teamB_bishop_2_y, 0);
            glScalef(0.5, 0.5, 0.5);
            glutSolidCone(0.75, 1, 50, 50);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(140.0 / 255.0, 140.0 / 255.0, 135.0 / 255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamB_queen_1_x, teamB_queen_1_y, 0.3);
            glScalef(0.5, 0.5, 0.5);
            glutSolidTetrahedron();
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(140.0 / 255.0, 140.0 / 255.0, 135.0 / 255.0);
            glRotatef(rot, 0.0f, 0.0f, 1.0f);
            glTranslated(teamB_king_2_x, teamB_king_2_y, 0.5);
            glScalef(0.5, 0.5, 0.5);
            glutSolidOctahedron();
            glPopMatrix();
            
        }
    }
    
    
// allowing double buffering
    
    glutSwapBuffers();
}
/*
void pressSpecialKey(int key, int xx, int yy)
{
    switch (key)
    {
        case GLUT_KEY_UP: deltaMove = 1.0; break;
        case GLUT_KEY_DOWN: deltaMove = -1.0; break;
    }
}

void releaseSpecialKey(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_UP: deltaMove = 0.0; break;
        case GLUT_KEY_DOWN: deltaMove = 0.0; break;
    }
}
*/


void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 'd':
            //day = (day + 10) % 360;
            glutPostRedisplay();
            break;
        case 'D':
            //day = (day - 10) % 360;
            glutPostRedisplay();
            break;
        case 'y':
            //year = (year + 5) % 360;
            glutPostRedisplay();
            break;
        case 'Y':
            //year = (year - 5) % 360;
            glutPostRedisplay();
            break;
        default:
            break;
    }
}
void update(void)
{
    if (deltaMove) { // update camera position
        x += deltaMove * lx * 0.1;
        y += deltaMove * ly * 0.1;
    }
    glutPostRedisplay(); // redisplay everything
}

void processNormalKeys(unsigned char key, int xx, int yy)
{
    if (key == ESC || key == 'q' || key == 'Q')
    {
        exit(0);
    }
    
    
    if (key == 'p' || key == 'P')
    {
        // choosing a random number between 0 and 15 to decide which pawn to move
        randPawn = rand() % 16;
        
        // determining the global positions of all pieces
        float pieces [32][2] = {{teamA_r1_x,teamA_r1_y}, {teamA_r2_x,teamA_r2_y} , {teamA_r3_x,teamA_r3_y}, {teamA_r4_x,teamA_r4_y} , {teamA_r5_x,teamA_r5_y} , {teamA_r6_x,teamA_r6_y}, {teamA_r7_x,teamA_r7_y}, {teamA_r8_x,teamA_r8_y},{teamA_rook_1_x, teamA_rook_1_y} , {teamA_rook_2_x, teamA_rook_2_y} , {teamA_knight_1_x, teamA_knight_1_y} ,{teamA_knight_2_x, teamA_knight_2_y},{teamA_bishop_1_x, teamA_bishop_1_y} ,{teamA_bishop_2_x, teamA_bishop_2_y} , {teamA_queen_1_x, teamA_queen_1_y} ,{teamA_king_2_x,teamA_king_2_y},{teamB_r1_x,teamB_r1_y}, {teamB_r2_x,teamB_r2_y} , {teamB_r3_x,teamB_r3_y}, {teamB_r4_x,teamB_r4_y} , {teamB_r5_x,teamB_r5_y} , {teamB_r6_x,teamB_r6_y}, {teamB_r7_x,teamB_r7_y}, {teamB_r8_x,teamB_r8_y},{teamB_rook_1_x, teamB_rook_1_y} , {teamB_rook_2_x, teamB_rook_2_y} , {teamB_knight_1_x, teamB_knight_1_y} ,{teamB_knight_2_x, teamB_knight_2_y},{teamB_bishop_1_x, teamB_bishop_1_y} ,{teamB_bishop_2_x, teamB_bishop_2_y},{teamB_queen_1_x, teamB_queen_1_y} ,{teamB_king_2_x,teamB_king_2_y}};
        
        // possible pawn moves
        int pawnMoves[2][2] = {{0,1}, {0,-1}};

        float nextPawnMove[2] ;
        nextPawnMove[0] = 0 ;
        nextPawnMove[1] = 0 ;
        // flag to store if the move is valid or not
        bool isItValidPawnMove ;
        // determining which pawn to move based on the outcome of the random number above
        if (randPawn == 0)
        {
            int j = 0;
            while (j < 2)
            {
                nextPawnMove[0] = teamA_r1_x + pawnMoves[j][0];
                nextPawnMove[1] = teamA_r1_y + pawnMoves[j][1];
                
                isItValidPawnMove = validPawnMove(nextPawnMove, pieces);
                if(isItValidPawnMove)
                {
                    teamA_r1_x = nextPawnMove[0] ;
                    teamA_r1_y = nextPawnMove[1] ;
                    break;
                }
                j++;
            }

        }
        
        if (randPawn == 1)
        {
            int j = 0;
            while (j < 2)
            {
                nextPawnMove[0] = teamA_r2_x + pawnMoves[j][0];
                nextPawnMove[1] = teamA_r2_y + pawnMoves[j][1];
                
                isItValidPawnMove = validPawnMove(nextPawnMove, pieces);
                if(isItValidPawnMove)
                {
                    teamA_r2_x = nextPawnMove[0] ;
                    teamA_r2_y = nextPawnMove[1] ;
                    break;
                }
                j++;
            }

        }
        
        
        if (randPawn == 2)
        {
            int j = 0;
            while (j < 2)
            {
                nextPawnMove[0] = teamA_r3_x + pawnMoves[j][0];
                nextPawnMove[1] = teamA_r3_y + pawnMoves[j][1];
                
                isItValidPawnMove = validPawnMove(nextPawnMove, pieces);
                if(isItValidPawnMove)
                {
                    teamA_r3_x = nextPawnMove[0] ;
                    teamA_r3_y = nextPawnMove[1] ;
                    break;
                }
                j++;
            }

        }
        
        if (randPawn == 3)
        {
            int j = 0;
            while (j < 2)
            {
                nextPawnMove[0] = teamA_r4_x + pawnMoves[j][0];
                nextPawnMove[1] = teamA_r4_y + pawnMoves[j][1];
                
                isItValidPawnMove = validPawnMove(nextPawnMove, pieces);
                if(isItValidPawnMove)
                {
                    teamA_r4_x = nextPawnMove[0] ;
                    teamA_r4_y = nextPawnMove[1] ;
                    break;
                }
                j++;
            }
        }
        
        if (randPawn == 4)
        {
            int j = 0;
            while (j < 2)
            {
                nextPawnMove[0] = teamA_r5_x + pawnMoves[j][0];
                nextPawnMove[1] = teamA_r5_y + pawnMoves[j][1];
                
                isItValidPawnMove = validPawnMove(nextPawnMove, pieces);
                if(isItValidPawnMove)
                {
                    teamA_r5_x = nextPawnMove[0] ;
                    teamA_r5_y = nextPawnMove[1] ;
                    break;
                }
                j++;
            }

        }
        if (randPawn == 5 )
        {
            int j = 0;
            while (j < 2)
            {
                nextPawnMove[0] = teamA_r6_x + pawnMoves[j][0];
                nextPawnMove[1] = teamA_r6_y + pawnMoves[j][1];
                
                isItValidPawnMove = validPawnMove(nextPawnMove, pieces);
                if(isItValidPawnMove)
                {
                    teamA_r6_x = nextPawnMove[0] ;
                    teamA_r6_y = nextPawnMove[1] ;
                    break;
                }
                j++;
            }

        }

        if (randPawn == 6 )
        {
            int j = 0;
            while (j < 2)
            {
                nextPawnMove[0] = teamA_r7_x + pawnMoves[j][0];
                nextPawnMove[1] = teamA_r7_y + pawnMoves[j][1];
                
                isItValidPawnMove = validPawnMove(nextPawnMove, pieces);
                if(isItValidPawnMove)
                {
                    teamA_r7_x = nextPawnMove[0] ;
                    teamA_r7_y = nextPawnMove[1] ;
                    break;
                }
                j++;
            }

        }
        
        if (randPawn == 7)
        {
            int j = 0;
            while (j < 2)
            {
                nextPawnMove[0] = teamA_r8_x + pawnMoves[j][0];
                nextPawnMove[1] = teamA_r8_y + pawnMoves[j][1];
                
                isItValidPawnMove = validPawnMove(nextPawnMove, pieces);
                if(isItValidPawnMove)
                {
                    teamA_r8_x = nextPawnMove[0] ;
                    teamA_r8_y = nextPawnMove[1] ;
                    break;
                }
                j++;
            }
        }
        
        if (randPawn == 8)
        {
            int j = 0;
            while (j < 2)
            {
                nextPawnMove[0] = teamB_r1_x + pawnMoves[j][0];
                nextPawnMove[1] = teamB_r1_y + pawnMoves[j][1];
                
                isItValidPawnMove = validPawnMove(nextPawnMove, pieces);
                if(isItValidPawnMove)
                {
                    teamB_r1_x = nextPawnMove[0] ;
                    teamB_r1_y = nextPawnMove[1] ;
                    break;
                }
                j++;
            }

        }
        
        if (randPawn == 9)
        {
            int j = 0;
            while (j < 2)
            {
                nextPawnMove[0] = teamB_r2_x + pawnMoves[j][0];
                nextPawnMove[1] = teamB_r2_y + pawnMoves[j][1];
                
                isItValidPawnMove = validPawnMove(nextPawnMove, pieces);
                if(isItValidPawnMove)
                {
                    teamB_r2_x = nextPawnMove[0] ;
                    teamB_r2_y = nextPawnMove[1] ;
                    break;
                }
                j++;
            }

        }
        
        if (randPawn == 10)
        {
            int j = 0;
            while (j < 2)
            {
                nextPawnMove[0] = teamB_r3_x + pawnMoves[j][0];
                nextPawnMove[1] = teamB_r3_y + pawnMoves[j][1];
                
                isItValidPawnMove = validPawnMove(nextPawnMove, pieces);
                if(isItValidPawnMove)
                {
                    teamB_r3_x = nextPawnMove[0] ;
                    teamB_r3_y = nextPawnMove[1] ;
                    break;
                }
                j++;
            }

        }
        
        if (randPawn == 11)
        {
            int j = 0;
            while (j < 2)
            {
                nextPawnMove[0] = teamB_r4_x + pawnMoves[j][0];
                nextPawnMove[1] = teamB_r4_y + pawnMoves[j][1];
                
                isItValidPawnMove = validPawnMove(nextPawnMove, pieces);
                if(isItValidPawnMove)
                {
                    teamB_r4_x = nextPawnMove[0] ;
                    teamB_r4_y = nextPawnMove[1] ;
                    break;
                }
                j++;
            }

        }
        
        if (randPawn == 12)
        {
            int j = 0;
            while (j < 2)
            {
                nextPawnMove[0] = teamB_r5_x + pawnMoves[j][0];
                nextPawnMove[1] = teamB_r5_y + pawnMoves[j][1];
                
                isItValidPawnMove = validPawnMove(nextPawnMove, pieces);
                if(isItValidPawnMove)
                {
                    teamB_r5_x = nextPawnMove[0] ;
                    teamB_r5_y = nextPawnMove[1] ;
                    break;
                }
                j++;
            }
        }
        
        if (randPawn == 13)
        {
            int j = 0;
            while (j < 2)
            {
                nextPawnMove[0] = teamB_r6_x + pawnMoves[j][0];
                nextPawnMove[1] = teamB_r6_y + pawnMoves[j][1];
                
                isItValidPawnMove = validPawnMove(nextPawnMove, pieces);
                if(isItValidPawnMove)
                {
                    teamB_r6_x = nextPawnMove[0] ;
                    teamB_r6_y = nextPawnMove[1] ;
                    break;
                }
                j++;
            }

        }
        
        if (randPawn == 14)
        {
            int j = 0;
            while (j < 2)
            {
                nextPawnMove[0] = teamB_r7_x + pawnMoves[j][0];
                nextPawnMove[1] = teamB_r7_y + pawnMoves[j][1];
                
                isItValidPawnMove = validPawnMove(nextPawnMove, pieces);
                if(isItValidPawnMove)
                {
                    teamB_r7_x = nextPawnMove[0] ;
                    teamB_r7_y = nextPawnMove[1] ;
                    break;
                }
                j++;
            }
 
        }
        
        if (randPawn == 15)
        {
            int j = 0;
            while (j < 2)
            {
                nextPawnMove[0] = teamB_r8_x + pawnMoves[j][0];
                nextPawnMove[1] = teamB_r8_y + pawnMoves[j][1];
                
                isItValidPawnMove = validPawnMove(nextPawnMove, pieces);
                if(isItValidPawnMove)
                {
                    teamB_r8_x = nextPawnMove[0] ;
                    teamB_r8_y = nextPawnMove[1] ;
                    break;
                }
                j++;
            }
        }

    }
    // if the user wants to move a random knight
    if (key == 'k' || key == 'K')
    {
        // determining the global positions of all pieces
        float pieces [32][2] = {{teamA_r1_x,teamA_r1_y}, {teamA_r2_x,teamA_r2_y} , {teamA_r3_x,teamA_r3_y}, {teamA_r4_x,teamA_r4_y} , {teamA_r5_x,teamA_r5_y} , {teamA_r6_x,teamA_r6_y}, {teamA_r7_x,teamA_r7_y}, {teamA_r8_x,teamA_r8_y},{teamA_rook_1_x, teamA_rook_1_y} , {teamA_rook_2_x, teamA_rook_2_y} , {teamA_knight_1_x, teamA_knight_1_y} ,{teamA_knight_2_x, teamA_knight_2_y},{teamA_bishop_1_x, teamA_bishop_1_y} ,{teamA_bishop_2_x, teamA_bishop_2_y} , {teamA_queen_1_x, teamA_queen_1_y} ,{teamA_king_2_x,teamA_king_2_y},{teamB_r1_x,teamB_r1_y}, {teamB_r2_x,teamB_r2_y} , {teamB_r3_x,teamB_r3_y}, {teamB_r4_x,teamB_r4_y} , {teamB_r5_x,teamB_r5_y} , {teamB_r6_x,teamB_r6_y}, {teamB_r7_x,teamB_r7_y}, {teamB_r8_x,teamB_r8_y},{teamB_rook_1_x, teamB_rook_1_y} , {teamB_rook_2_x, teamB_rook_2_y} , {teamB_knight_1_x, teamB_knight_1_y} ,{teamB_knight_2_x, teamB_knight_2_y},{teamB_bishop_1_x, teamB_bishop_1_y} ,{teamB_bishop_2_x, teamB_bishop_2_y},{teamB_queen_1_x, teamB_queen_1_y} ,{teamB_king_2_x,teamB_king_2_y}};
        
        
        // array to store all possible moves by knight
        int knightMoves[8][2] = {{1,2}, {1,-2}, {2,1}, {-2,1}, {-1,2}, {-1,-2}, {2,-1}, {-2,-1}};

        float nextKnightMove[2] ;
        nextKnightMove[0] = 0 ;
        nextKnightMove[1] = 0 ;
        bool isItValidMove ;
        // choosing a random number from 0 to 3
        randKnight = rand() % 4;
        // moving 1 of the 4 knights depending on the outcome of the random numbers chosen
        if (randKnight == 0)
        {
            int i = 0;
            while (i < 8)
            {
                nextKnightMove[0] = teamA_knight_1_x + knightMoves[i][0];
                nextKnightMove[1] = teamA_knight_1_y + knightMoves[i][1];
                // determining if the move is valid or not
                isItValidMove = validKnightMove(nextKnightMove, pieces);
                if(isItValidMove)
                {
                    teamA_knight_1_x = nextKnightMove[0] ;
                    teamA_knight_1_y = nextKnightMove[1] ;
                    break;
                }
                i++;
            }
        }
        
        if (randKnight == 1)
        {
            int i = 0;
            while (i < 8)
            {
                nextKnightMove[0] = teamA_knight_2_x + knightMoves[i][0];
                nextKnightMove[1] = teamA_knight_2_y + knightMoves[i][1];
                // determining if the move is valid or not

                isItValidMove = validKnightMove(nextKnightMove, pieces);
                if(isItValidMove)
                {
                    teamA_knight_2_x = nextKnightMove[0] ;
                    teamA_knight_2_y = nextKnightMove[1] ;
                    break;
                }
                i++;
            }
        }
        
        if (randKnight == 2)
        {
            int i = 0;
            while (i < 8)
            {
                nextKnightMove[0] = teamB_knight_1_x + knightMoves[i][0];
                nextKnightMove[1] = teamB_knight_1_y + knightMoves[i][1];
                // determining if the move is valid or not

                isItValidMove = validKnightMove(nextKnightMove, pieces);
                if(isItValidMove)
                {
                    teamB_knight_1_x = nextKnightMove[0] ;
                    teamB_knight_1_y = nextKnightMove[1] ;
                    break;
                }
                i++;
            }
        }
        
        if (randKnight == 3)
        {
            int i = 0;
            while (i < 8)
            {
                nextKnightMove[0] = teamB_knight_2_x + knightMoves[i][0];
                nextKnightMove[1] = teamB_knight_2_y + knightMoves[i][1];
                // determining if the move is valid or not

                isItValidMove = validKnightMove(nextKnightMove, pieces);
                if(isItValidMove)
                {
                    teamB_knight_2_x = nextKnightMove[0] ;
                    teamB_knight_2_y = nextKnightMove[1] ;
                    break;
                }
                i++;
            }
        }

    }
    
    // if zooming out
    
    if (key == 'd' || key == 'D' )
    {
        eye_z -= 0.25;
        glutPostRedisplay();

    }
    // if accessing enhanced mode
    if (key == 'e' || key == 'E' )
    {

        enhancedMode = !enhancedMode;
        glutPostRedisplay();
        
    }
    // if zooming in

    if (key == 'u' || key == 'U' )
    {
        eye_z += 0.25;
        //gluLookAt(4, -5, eye_z+0.25, 4 ,  4, 0,0, 0, 10);
        glutPostRedisplay();
        
    }
    // if rotating the board
    if (key == 'r' || key == 'R' )
    {
        rot += 10;
        //gluLookAt(4, -5, eye_z+0.25, 4 ,  4, 0,0, 0, 10);
        glutPostRedisplay();
        
    }
    // turning off/on GLLight0
    if (key == '0' )
    {

        GLLight0_flag = !GLLight0_flag;
        glutPostRedisplay();
    }
    
    // turning off/on GLLight1

    if (key == '1')
    {

        GLLight1_flag = !GLLight1_flag;
        glutPostRedisplay();
    }

}


/*
 Below is main function; it calls for the display, and initilizes the window size, and calls for the necessary functions to take keyboard strokes
 
 
 */
int main(int argc, char** argv)
{
    // initailzing the board
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("The awesome Hmk6 Chessboard Game!!!!");
    glutReshapeFunc(changeSize); // window reshape callback
    glEnable(GL_NORMALIZE);
    glutDisplayFunc(myDisplay);
    
    glutIdleFunc(update); // incremental update
    glutIgnoreKeyRepeat(1); // ignore key repeat when holding key down
    //glutMouseFunc(mouseButton); // process mouse button push/release
    //glutMotionFunc(mouseMove); // process mouse dragging motion
    
    // processing keys
    
    glutKeyboardFunc(processNormalKeys); // process standard key clicks
    //glutSpecialFunc(pressSpecialKey); // process special key pressed
    // Warning: Nonstandard function! Delete if desired.
    //glutSpecialUpFunc(releaseSpecialKey); // process special key release
    //init();
    //glutKeyboardFunc(keyboard);
   
    glutMainLoop();
    return 0;
}
