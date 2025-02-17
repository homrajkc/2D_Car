#include <windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <cstdio> // For sprintf
#include <cmath>  // For abs

// Game speed
int FPS = 30;

// Game track
int start = 0;
int gv = 0;
int level = 0;

// Track score
int score = 0;

// Road division positions
int roadDivTopMost = 0;
int roadDivTop = 0;
int roadDivMd1 = 0;
int roadDivBtm = 0;

// Car left-right position
int lrIndex = 0;

// Incoming cars
int car1 = 0;
int lrIndex1 = 0;
int car2 = 35;
int lrIndex2 = 0;
int car3 = 70;
int lrIndex3 = 0;

// Fonts for text rendering
const void* font1 = GLUT_BITMAP_TIMES_ROMAN_24;
const void* font2 = GLUT_BITMAP_HELVETICA_18;
const void* font3 = GLUT_BITMAP_8_BY_13;

// Function to render text on the screen
void renderBitmapString(float x, float y,  const void* font, const char* string) {
    const char* c;
    glRasterPos2f(x, y);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter((void*)font, *c);
    }
}

// Function to draw a tree
void tree(int x, int y) {
    int newx = x;
    int newy = y;

    // Tree trunk
    glColor3f(0.871, 0.722, 0.529);
    glBegin(GL_TRIANGLES);
    glVertex2f(newx + 11, newy + 55);
    glVertex2f(newx + 12, newy + 55 - 10);
    glVertex2f(newx + 10, newy + 55 - 10);
    glEnd();

    // Tree leaves
    glColor3f(0.133, 0.545, 0.133);
    glBegin(GL_TRIANGLES);
    glVertex2f(newx + 11, newy + 55 + 3);
    glVertex2f(newx + 12 + 3, newy + 55 - 3);
    glVertex2f(newx + 10 - 3, newy + 55 - 3);
    glEnd();
}

// Function to handle the main game logic
void startGame() {
    // Road background
    glColor3f(0.412, 0.412, 0.412);
    glBegin(GL_POLYGON);
    glVertex2f(20, 0);
    glVertex2f(20, 100);
    glVertex2f(80, 100);
    glVertex2f(80, 0);
    glEnd();

    // Road left border
    glColor3f(1.000, 1.000, 1.000);
    glBegin(GL_POLYGON);
    glVertex2f(20, 0);
    glVertex2f(20, 100);
    glVertex2f(23, 100);
    glVertex2f(23, 0);
    glEnd();

    // Road right border
    glColor3f(1.000, 1.000, 1.000);
    glBegin(GL_POLYGON);
    glVertex2f(77, 0);
    glVertex2f(77, 100);
    glVertex2f(80, 100);
    glVertex2f(80, 0);
    glEnd();

    // Road middle border (top)
    glColor3f(1.000, 1.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(48, roadDivTop + 80);
    glVertex2f(48, roadDivTop + 100);
    glVertex2f(52, roadDivTop + 100);
    glVertex2f(52, roadDivTop + 80);
    glEnd();
    roadDivTop--;
    if (roadDivTop < -100) {
        roadDivTop = 20;
        score++;
    }

    // Road middle border (middle)
    glColor3f(1.000, 1.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(48, roadDivMd1 + 40);
    glVertex2f(48, roadDivMd1 + 60);
    glVertex2f(52, roadDivMd1 + 60);
    glVertex2f(52, roadDivMd1 + 40);
    glEnd();
    roadDivMd1--;
    if (roadDivMd1 < -60) {
        roadDivMd1 = 60;
        score++;
    }

    // Road middle border (bottom)
    glColor3f(1.000, 1.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(48, roadDivBtm + 0);
    glVertex2f(48, roadDivBtm + 20);
    glVertex2f(52, roadDivBtm + 20);
    glVertex2f(52, roadDivBtm + 0);
    glEnd();
    roadDivBtm--;
    if (roadDivBtm < -20) {
        roadDivBtm = 100;
        score++;
    }

    // Score board
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(80, 97);
    glVertex2f(100, 97);
    glVertex2f(100, 90);
    glVertex2f(80, 90);
    glEnd();

    // Display score
    char buffer[50];
    sprintf(buffer, "SCORE: %d", score);
    glColor3f(0.000, 1.000, 0.000);
    renderBitmapString(80.5, 95, font3, buffer);

    // Display speed
    char buffer1[50];
    sprintf(buffer1, "SPEED: %d Km/h", FPS);
    glColor3f(0.000, 1.000, 0.000);
    renderBitmapString(80.5, 92, font3, buffer1);

    // Increase level and speed
    if (score % 50 == 0) {
        int last = score / 50;
        if (last != level) {
            level = score / 50;
            FPS += 2;
        }
    }

    // Display level
    char level_buffer[50];
    sprintf(level_buffer, "LEVEL: %d", level);
    glColor3f(0.000, 1.000, 0.000);
    renderBitmapString(80.5, 89, font3, level_buffer);

    // Main car
    // Front
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex + 26 - 2, 5);
    glVertex2f(lrIndex + 26 - 2, 7);
    glVertex2f(lrIndex + 30 + 2, 7);
    glVertex2f(lrIndex + 30 + 2, 5);
    glEnd();

    // Back
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex + 26 - 2, 1);
    glVertex2f(lrIndex + 26 - 2, 3);
    glVertex2f(lrIndex + 30 + 2, 3);
    glVertex2f(lrIndex + 30 + 2, 1);
    glEnd();

    // Car body
    glColor3f(0.678, 1.000, 0.184);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex + 26, 1);
    glVertex2f(lrIndex + 26, 8);
    glVertex2f(lrIndex + 28, 10);
    glVertex2f(lrIndex + 30, 8);
    glVertex2f(lrIndex + 30, 1);
    glEnd();

    // Incoming car 1
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex1 + 26 - 2, car1 + 100 - 4);
    glVertex2f(lrIndex1 + 26 - 2, car1 + 100 - 6);
    glVertex2f(lrIndex1 + 30 + 2, car1 + 100 - 6);
    glVertex2f(lrIndex1 + 30 + 2, car1 + 100 - 4);
    glEnd();

    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex1 + 26 - 2, car1 + 100);
    glVertex2f(lrIndex1 + 26 - 2, car1 + 100 - 2);
    glVertex2f(lrIndex1 + 30 + 2, car1 + 100 - 2);
    glVertex2f(lrIndex1 + 30 + 2, car1 + 100);
    glEnd();

    glColor3f(1.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex1 + 26, car1 + 100);
    glVertex2f(lrIndex1 + 26, car1 + 100 - 7);
    glVertex2f(lrIndex1 + 28, car1 + 100 - 9);
    glVertex2f(lrIndex1 + 30, car1 + 100 - 7);
    glVertex2f(lrIndex1 + 30, car1 + 100);
    glEnd();
    car1--;
    if (car1 < -100) {
        car1 = 0;
        lrIndex1 = lrIndex;
    }

    // Check collision with car 1
    if ((abs(lrIndex - lrIndex1)) < 8 && (car1 + 100 < 10)) {
        start = 0;
        gv = 1;
    }

    // Incoming car 2
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex2 + 26 - 2, car2 + 100 - 4);
    glVertex2f(lrIndex2 + 26 - 2, car2 + 100 - 6);
    glVertex2f(lrIndex2 + 30 + 2, car2 + 100 - 6);
    glVertex2f(lrIndex2 + 30 + 2, car2 + 100 - 4);
    glEnd();

    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex2 + 26 - 2, car2 + 100);
    glVertex2f(lrIndex2 + 26 - 2, car2 + 100 - 2);
    glVertex2f(lrIndex2 + 30 + 2, car2 + 100 - 2);
    glVertex2f(lrIndex2 + 30 + 2, car2 + 100);
    glEnd();

    glColor3f(0.294, 0.000, 0.510);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex2 + 26, car2 + 100);
    glVertex2f(lrIndex2 + 26, car2 + 100 - 7);
    glVertex2f(lrIndex2 + 28, car2 + 100 - 9);
    glVertex2f(lrIndex2 + 30, car2 + 100 - 7);
    glVertex2f(lrIndex2 + 30, car2 + 100);
    glEnd();
    car2--;
    if (car2 < -100) {
        car2 = 0;
        lrIndex2 = lrIndex;
    }

    // Check collision with car 2
    if ((abs(lrIndex - lrIndex2)) < 8 && (car2 + 100 < 10)) {
        start = 0;
        gv = 1;
    }

    // Incoming car 3
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex3 + 26 - 2, car3 + 100 - 4);
    glVertex2f(lrIndex3 + 26 - 2, car3 + 100 - 6);
    glVertex2f(lrIndex3 + 30 + 2, car3 + 100 - 6);
    glVertex2f(lrIndex3 + 30 + 2, car3 + 100 - 4);
    glEnd();

    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex3 + 26 - 2, car3 + 100);
    glVertex2f(lrIndex3 + 26 - 2, car3 + 100 - 2);
    glVertex2f(lrIndex3 + 30 + 2, car3 + 100 - 2);
    glVertex2f(lrIndex3 + 30 + 2, car3 + 100);
    glEnd();

    glColor3f(1.000, 0.271, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex3 + 26, car3 + 100);
    glVertex2f(lrIndex3 + 26, car3 + 100 - 7);
    glVertex2f(lrIndex3 + 28, car3 + 100 - 9);
    glVertex2f(lrIndex3 + 30, car3 + 100 - 7);
    glVertex2f(lrIndex3 + 30, car3 + 100);
    glEnd();
    car3--;
    if (car3 < -100) {
        car3 = 0;
        lrIndex3 = lrIndex;
    }

    // Check collision with car 3
    if ((abs(lrIndex - lrIndex3)) < 8 && (car3 + 100 < 10)) {
        start = 0;
        gv = 1;
    }
}

// Function to draw the initial game screen
void firstDesign() {
    // Background
    glColor3f(0.000, 0.392, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(0, 55);
    glVertex2f(100, 55);
    glColor3f(0.604, 0.804, 0.196);
    glVertex2f(100, 0);
    glVertex2f(0, 0);
    glEnd();

    // Road sky
    glColor3f(0.000, 0.749, 1.000);
    glBegin(GL_POLYGON);
    glVertex2f(100, 100);
    glVertex2f(0, 100);
    glColor3f(0.686, 0.933, 0.933);
    glVertex2f(0, 55);
    glVertex2f(100, 55);
    glEnd();

    // Trees
    tree(5, -15);
    tree(9, 5);
    tree(95, 9);
    tree(75, -5);

    // Game over screen
    if (gv == 1) {
        glColor3f(1.000, 0.000, 0.000);
        renderBitmapString(35, 60, font1, "GAME OVER");
        char buffer2[50];
        sprintf(buffer2, "Your score is: %d", score);
        renderBitmapString(33, 55, font1, buffer2);
    }

    // Game title and instructions
    glColor3f(1.000, 1.000, 0.000);
    renderBitmapString(30, 80, font1, "2D Car Racing Game");

    glColor3f(0.000, 1.000, 0.000);
    renderBitmapString(30, 50, font2, "Press SPACE to START");
    renderBitmapString(30, 45, font2, "Press ESC to Exit");

    glColor3f(1.000, 1.000, 1.000);
    renderBitmapString(30, 40, font3, "Press UP to increase speed");
    renderBitmapString(30, 37, font3, "Press DOWN to decrease speed");
    renderBitmapString(30, 34, font3, "Press RIGHT to turn right");
    renderBitmapString(30, 31, font3, "Press LEFT to turn left");
}

// Display function
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (start == 1) {
        glClearColor(0.000, 0.392, 0.000, 1);
        startGame();
    } else {
        firstDesign();
    }

    glFlush();
    glutSwapBuffers();
}

// Special key input handling
void spe_key(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_DOWN:
            if (FPS > (50 + (level * 2))) FPS -= 2;
            break;
        case GLUT_KEY_UP:
            FPS += 2;
            break;
        case GLUT_KEY_LEFT:
            if (lrIndex >= 0) {
                lrIndex -= (FPS / 10);
                if (lrIndex < 0) lrIndex = 0;
            }
            break;
        case GLUT_KEY_RIGHT:
            if (lrIndex <= 44) {
                lrIndex += (FPS / 10);
                if (lrIndex > 44) lrIndex = 44;
            }
            break;
        default:
            break;
    }
}

// Regular key input handling
void processKeys(unsigned char key, int x, int y) {
    switch (key) {
        case ' ':
            if (start == 0) {
                start = 1;
                gv = 0;
                FPS = 50;
                roadDivTopMost = 0;
                roadDivTop = 0;
                roadDivMd1 = 0;
                roadDivBtm = 0;
                lrIndex = 0;
                car1 = 0;
                lrIndex1 = 0;
                car2 = 35;
                lrIndex2 = 0;
                car3 = 70;
                lrIndex3 = 0;
                score = 0;
                level = 0;
            }
            break;
        case 27: // ESC key
            exit(0);
            break;
        default:
            break;
    }
}

// Timer function for game updates
void timer(int) {
    glutPostRedisplay();
    glutTimerFunc(1000 / FPS, timer, 0);
}

// Main function
int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(500, 650);
    glutInitWindowPosition(200, 20);
    glutCreateWindow("Car Racing Game");

    glutDisplayFunc(display);
    glutSpecialFunc(spe_key);
    glutKeyboardFunc(processKeys);

    glOrtho(0, 100, 0, 100, -1, 1);
    glClearColor(0.184, 0.310, 0.310, 1);

    glutTimerFunc(1000, timer, 0);
    glutMainLoop();

    return 0;
}