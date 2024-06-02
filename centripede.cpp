#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include <cstdlib>

using namespace std;

const int resolutionX = 960;
const int resolutionY = 960;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionX / boxPixelsX;
const int gameColumns = resolutionY / boxPixelsY;

const int x = 0;
const int y = 1;
const int exists = 2;
// Function declarations
void drawCentipede(sf::RenderWindow& window, float centipede[][5], sf::Sprite& centipedeSprite, sf::Sprite& centipedehead, int segments, float mushroomarr[][3], int num, const int& player_area, const int& direction); //it draws centipede segment or initial head or head after being hit by bullet on its index 
void moveCentipede(float centipede[][5], int segments, sf::Clock& centipedeClock, float mushroomarr[][3], int num, const int& player_area, const int& direction, float player[], sf::RenderWindow& window, sf::Text& scoreText, sf::Font& font2); //it checks for collision among centipede and any object and then its controls speed and moves centipede up or down according to its position
void handleInput(float player[], float bullet[], const int& player_area, const int& direction); // it controls player movement and shooting
void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite); //player is drawn on window
void moveBullet(sf::RenderWindow& window, float bullet[], sf::Clock& bulletClock, float centipede[][5], sf::Sprite& headSprite, int segments, int num, float mushroombroke[][3], float mushroomarr[][3], sf::Sprite& mushroom, const int& player_area, const int& direction, int& score);//one of the important functions that handle bullet collision with centipede ,mushroom and broken mushroom
void drawBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& bulletSprite);//bullet is drawn on window
void drawmashroom(sf::RenderWindow& window, float mushroomarr[][3], sf::Sprite& mushroom, int num, float mushroombroke[][3], sf::Sprite& broke, const int& player_area, const int& direction); //mushroom appaer depending on the lives of mushroom and also changes colour of muhsroom depending upon its position of appearance in game grid
void setmushroom(float mushroomarr[][3], int num, float mushroombroke[][3], const int& player_area, const int& direction); // random mushrooms positions are set in this code and mushrooms for centipede hit are also set in it
void updateSegmentPositions(float centipede[][5], int segments, const int& player_area, const int& direction); // it handles speed and downward movement of centipede and also its back and forth movement in player area
void checkForCollisions(float centipede[][5], int segments, float mushroomarr[][3], int num, const int& player_area, const int& direction, float player[], sf::RenderWindow& window, sf::Text& scoreText, sf::Font& font2); // collision of centipede with mushroom
void gameover(sf::RenderWindow& window, sf::Text& scoreText, sf::Font& font2); //it player is killed game is over

int main() {

    // Constants for player direction and area
    const int direction = 3;
    const int player_area = 9;


    srand(time(0));//  random number generator


    sf::Font font2;// Declared a font object 


    sf::Text scoreText2;// Declared a text object for displaying the score


    font2.loadFromFile("SpritzAndDelicious_PERSONAL_USE_ONLY.otf");// Loaded a font file from the specified path

    // Set the font for the score text
    scoreText2.setFont(font2);
    scoreText2.setCharacterSize(2 * boxPixelsX);

    // Set the fill green color for the score text 
    scoreText2.setFillColor(sf::Color::Green);
    // Set the position of the score text on the window
    scoreText2.setPosition(200, 200);



    sf::Font font;// Declared a font object


    sf::Text scoreText;// Declared a text object for displaying the score


    font.loadFromFile("Alinore.otf");// Loaded a font file from the specified path


    scoreText.setFont(font);// Set the font for the score text

    scoreText.setCharacterSize(1 * boxPixelsX);// Set the character size for the score text (1 times the width of a game box)

    scoreText.setFillColor(sf::Color::Green);// Set the fill color geen  for the score text 


    scoreText.setPosition(10, 10);// Set the position of the score text on the window


    int score = 0;// Initialize the score variable with 0,muhsroom+5,centipede+10


    sf::RenderWindow window(sf::VideoMode(resolutionX, resolutionY), "Centipede", sf::Style::Close | sf::Style::Titlebar);
    window.setSize(sf::Vector2u(1850, 1000));
    //   window.setSize(sf::Vector2u(640, 640)); 
    window.setPosition(sf::Vector2i(0, 0));

    sf::Music bgMusic;
    bgMusic.openFromFile("Music/field_of_hopes.ogg");
    bgMusic.play();
    bgMusic.setVolume(50);

    sf::Texture backgroundTexture;   // Declared a texture object to hold the background image


    sf::Sprite backgroundSprite;// Declared a sprite object to represent the background


    backgroundTexture.loadFromFile("Textures/Untitled Project(3).jpg");// Loaded the background image from the specified file path

    backgroundSprite.setTexture(backgroundTexture);// Set the texture for the background sprite

    // Set the color of the background sprite with transparency
    //  transparency level (0 = fully transparent, 255 = fully opaque)
    backgroundSprite.setColor(sf::Color(255, 255, 255, 255 * 0.50));


    float player[3] = {};
    player[x] = (gameColumns / 2) * boxPixelsX;
    player[y] = 25 * boxPixelsY;
    player[exists] = true;

    sf::Texture playerTexture;
    sf::Sprite playerSprite;
    playerTexture.loadFromFile("Textures/player.png");
    playerSprite.setTexture(playerTexture);
    playerSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

    float bullet[3] = {};
    bullet[x] = player[x];
    bullet[y] = player[y] - boxPixelsY;
    bullet[exists] = false;
    sf::Clock bulletClock;
    sf::Texture bulletTexture;
    sf::Sprite bulletSprite;
    bulletTexture.loadFromFile("Textures/bullet.png");
    bulletSprite.setTexture(bulletTexture);
    bulletSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

    const int centipedeSegments = 12;
    float centipede[centipedeSegments][5] = {}; // Array to store centipede segments

    // Initialize centipede array with exists flag set to true for each segment
    int b = rand() % 545 + 384; //range(384 or 12 to 928 to 29)
    b /= 32;
    for (int i = 0; i < centipedeSegments; ++i) {
        // Calculate the x-coordinate of the ith segment in the centipede
        centipede[i][x] = (b * boxPixelsX) - (i * boxPixelsX);


        centipede[i][y] = 0 * boxPixelsY;  // Set the y-coordinate of the ith segment in the centipede to 0


        centipede[i][exists] = true;// Set the existence flag for the ith segment to true

        // Set the direction flag for the ith segment to true (assuming this is a boolean property)
        centipede[i][direction] = true;

        // Set the player_area flag for the ith segment to false (assuming this is a boolean property)
        centipede[i][player_area] = false;
    }
    // Set Framerate Limit to 60 frames per second
    window.setFramerateLimit(60);

    // Centipede Body Sprite
    sf::Sprite centipedeSprite;
    sf::Texture centipedeTexture;
    centipedeTexture.loadFromFile("Textures/c_body_left_walk.png");  // Load texture from file
    centipedeSprite.setTexture(centipedeTexture);  // Set texture for the sprite
    centipedeSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));  // Set texture rectangle for display
    centipedeSprite.setColor(sf::Color(153, 204, 0));  // Set color tint to green

    // Centipede Head Sprite
    sf::Sprite centipedehead;
    sf::Texture centipedeHead;
    centipedeHead.loadFromFile("Textures/c_head_left_walk.png");  // Load texture from file
    centipedehead.setTexture(centipedeHead);  // Set texture for the sprite
    centipedehead.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));  // Set texture rectangle for display
    centipedehead.setColor(sf::Color(153, 204, 0));  // Set color tint to green

    // Mushroom Sprites
    sf::Sprite mushroom;
    sf::Sprite broke;
    sf::Texture Mushroom;
    Mushroom.loadFromFile("Textures/mushroom.png");  // Load texture from file
    mushroom.setTexture(Mushroom);  // Set texture for the sprite
    broke.setTexture(Mushroom);  // Set texture for the broke sprite (assuming this is intended for something specific)





    // const int numofmushrooms = rand() % 30 + 20;
    const int numofmushrooms = 30;
    float mushroomarray[numofmushrooms + 24][3] = {};
    float mushroombroke[numofmushrooms + 24][3] = {};
    setmushroom(mushroomarray, numofmushrooms, mushroombroke, exists, direction);

    sf::Clock centipedeClock;

    while (window.isOpen()) {
        window.draw(backgroundSprite);

        scoreText.setString("ScORe : " + std::to_string(score));
        window.draw(scoreText);

        drawmashroom(window, mushroomarray, mushroom, numofmushrooms, mushroombroke, broke, player_area, direction);
        handleInput(player, bullet, player_area, direction);



        drawPlayer(window, player, playerSprite);
        drawCentipede(window, centipede, centipedeSprite, centipedehead, centipedeSegments, mushroomarray, numofmushrooms, player_area, direction);
        moveCentipede(centipede, centipedeSegments, centipedeClock, mushroomarray, numofmushrooms, player_area, direction, player, window, scoreText2, font2);

        if (bullet[exists] == true) {
            moveBullet(window, bullet, bulletClock, centipede, centipedehead, centipedeSegments, numofmushrooms, mushroombroke, mushroomarray, mushroom, player_area, direction, score);
            drawBullet(window, bullet, bulletSprite);
        }

        window.display();
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                return 0;
            }
        }


        window.clear();
    }
}

void drawCentipede(sf::RenderWindow& window, float centipede[][5], sf::Sprite& centipedeSprite, sf::Sprite& headSprite, int segments, float mushroomarr[][3], int num, const int& player_area, const int& direction) {
    // Iterate over centipede segments
    for (int i = 0; i < segments; ++i) {
        // Check if the current segment exists and is not the last one
        if (centipede[i][exists] == true && i != segments - 1) {
            // Create a sprite for the current centipede segment
            sf::Sprite segmentSprite = centipedeSprite;
            segmentSprite.setPosition(centipede[i][x], centipede[i][y]);

            // Draw the centipede segment
            window.draw(segmentSprite);
        }

        // Check if the current segment does not exist, the previous one exists, and it's not the first segment
        if (centipede[i][exists] == false && i - 1 >= 0 && centipede[i - 1][exists] == true) {
            // Position the head sprite at the location of the last existing segment
            headSprite.setPosition(centipede[i - 1][x], centipede[i - 1][y]);

            // Draw the head sprite
            window.draw(headSprite);
        }

        // Check if it's the last segment and it exists
        if (i == segments - 1 && centipede[i][exists] == true) {
            // Position the head sprite at the location of the last segment
            if (centipede[i][exists]) {
                headSprite.setPosition(centipede[i][x], centipede[i][y]);

                // Draw the head sprite
                window.draw(headSprite);
            }
        }
    }
}



void moveCentipede(float centipede[][5], int segments, sf::Clock& centipedeClock, float mushroomarr[][3], int num, const int& player_area, const int& direction, float player[], sf::RenderWindow& window, sf::Text& scoreText, sf::Font& font2) {
    // Check if the time elapsed since the last centipede move is less than 100 milliseconds
    if (centipedeClock.getElapsedTime().asMilliseconds() < 100) {
        // If not enough time has passed, return without moving the centipede
        return;
    }

    centipedeClock.restart(); // Restart the clock to measure the time until the next move


    checkForCollisions(centipede, segments, mushroomarr, num, player_area, direction, player, window, scoreText, font2);    // Check for collisions with mushrooms, player, and update the score

    // Update the positions of centipede segments based on the current direction
    updateSegmentPositions(centipede, segments, player_area, direction);


}
void updateSegmentPositions(float centipede[][5], int segments, const int& player_area, const int& direction) {
    // Loop through each segment of the centipede
    for (int i = 0; i < segments; ++i) {
        // Check if the segment is at the bottom row
        if (centipede[i][y] == 29 * boxPixelsY) {
            // Set the player area flag to true if the segment is at the bottom
            centipede[i][player_area] = true;
        }
        // Check if the segment is at the row above the player area
        if (centipede[i][y] == 25 * boxPixelsY) {
            // Set the player area flag to false if the segment is at the row above
            centipede[i][player_area] = false;
        }

        // Move the segment based on its current direction
        if (centipede[i][direction]) {
            // Move left
            centipede[i][x] -= 1 * boxPixelsX;
        }
        else {
            // Move right
            centipede[i][x] += 1 * boxPixelsX;
        }

        // Check if the segment is out of bounds or at the edges of the window
        if (centipede[i][x] < 0 || centipede[i][x] == resolutionX) {
            // Check if the segment is not in the player area
            if (centipede[i][player_area] == false) {
                // Change direction and move down
                centipede[i][direction] = !centipede[i][direction];
                centipede[i][y] += boxPixelsY;

                // Adjust position if out of bounds
                if (centipede[i][x] < 0) {
                    centipede[i][x] += 1 * boxPixelsX;
                }
                if (centipede[i][x] == resolutionX) {
                    centipede[i][x] -= 1 * boxPixelsX;
                }
            }
            // Check if the segment is in the player area
            if (centipede[i][player_area] == true) {
                // Change direction and move up
                centipede[i][direction] = !centipede[i][direction];
                centipede[i][y] -= boxPixelsY;
            }
        }
    }
}
void checkForCollisions(float centipede[][5], int segments, float mushroomarr[][3], int num, const int& player_area, const int& direction, float player[], sf::RenderWindow& window, sf::Text& scoreText, sf::Font& font2) {
    // Loop through each mushroom and centipede segment
    for (int j = 0; j < num + 24; ++j) {
        for (int i = 0; i < segments; ++i) {
            // Check if the current mushroom is at the same position as the centipede segment
            if (mushroomarr[j][x] == centipede[i][x] && mushroomarr[j][y] == centipede[i][y])
            {
                // Move the centipede segment down and change direction upon collision with a mushroom
                centipede[i][y] += boxPixelsY;
                centipede[i][direction] = !centipede[i][direction];
            }

            // Check for collision between player and centipede segment
            if (player[x] >= centipede[i][x] && player[x] <= centipede[i][x] + boxPixelsX &&
                player[y] >= centipede[i][y] && player[y] <= centipede[i][y] + boxPixelsY &&
                player[exists] == true && centipede[i][exists] == true) {
                // Set player existence to false, trigger game over, and move player out of view
                player[exists] = false;
                gameover(window, scoreText, font2);
                player[x] = 1000;
                player[y] = 1000;
                break;  // Exit the loop after a collision
            }
        }

        // Check for collision between player and mushroom
        if (player[x] >= mushroomarr[j][x] && player[x] <= mushroomarr[j][x] + boxPixelsX &&
            player[y] >= mushroomarr[j][y] && player[y] <= mushroomarr[j][y] + boxPixelsY &&
            player[exists] == true && mushroomarr[j][exists] == true) {
            // Set player existence to false, trigger game over, and move player out of view
            player[exists] = false;
            gameover(window, scoreText, font2);
            player[x] = 1000;
            player[y] = 1000;
            break;  // Exit the loop after a collision
        }
    }
}


void handleInput(float player[], float bullet[], const int& player_area, const int& direction) {
    // Handle player movement based on keyboard input
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && player[x] > 0 && player[exists] == true) {  //player horizontal speed and movement 
        player[x] -= 10;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && player[x] < resolutionX - boxPixelsX && player[exists] == true) { //player horizontal speed and movement 
        player[x] += 10;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && player[y] > (gameRows - 5) * boxPixelsY && player[exists] == true) { //player  vertical speed and movement up to 5 rows
        player[y] -= 10; // Adjust the value for the desired speed
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && player[y] < resolutionY - boxPixelsY && player[exists] == true) { //player  vertical speed and movement
        player[y] += 10; // Adjust the value for the desired speed
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !bullet[exists] && player[exists] == true) {
        // Shoot a new bullet when the Space key is pressed and the previous bullet doesn't exist
        bullet[x] = player[x]; // same horizontal position
        bullet[y] = player[y] - boxPixelsY;// syncing player and bullet
        bullet[exists] = true;
    }
}

void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite) {
    // Set the position of the player sprite and draw it on the window
    playerSprite.setPosition(player[x], player[y]);
    window.draw(playerSprite);
}

void moveBullet(sf::RenderWindow& window, float bullet[], sf::Clock& bulletClock, float centipede[][5], sf::Sprite& headSprite, int segments, int num, float mushroombroke[][3], float mushroomarr[][3], sf::Sprite& mushroom, const int& player_area, const int& direction, int& score) {
    // Check if enough time has elapsed since the last bullet movement
    if (bulletClock.getElapsedTime().asMilliseconds() < 4)
        return;

    bulletClock.restart();

    // Move the bullet upwards at a fixed speed
    bullet[y] -= 32; // bullet speed

    // Check if the bullet has moved out of the screen
    if (bullet[y] < -boxPixelsY) {
        // Reset the bullet position and mark it as non-existent
        bullet[x] = 1000; // bullet displacement
        bullet[y] = 1000; // bullet displacement
        bullet[exists] = false;
    }





    for (int i = 0, j = num; i < segments, j < num + 24; ++i, j++) {
        // Check if the bullet is within a certain range of the centipede segment
        if (bullet[x] >= centipede[i][x] && bullet[x] <= centipede[i][x] + boxPixelsX &&
            bullet[y] >= centipede[i][y] && bullet[y] <= centipede[i][y] + boxPixelsY) { //bullet matched centipede 
            centipede[i][exists] = false;                // segment disappear
            score += 10;
            mushroomarr[j][exists] = true;   // mushroom appear
            mushroombroke[j][exists] = false;  //broken not
            mushroomarr[j][x] = centipede[i][x];  //position of normal mushroom
            mushroomarr[j][y] = centipede[i][y]; //position of normal mushroom
            mushroombroke[j][x] = centipede[i][x];//position of broken mushroom
            mushroombroke[j][y] = centipede[i][y]; //position of broken mushroom


            bullet[exists] = false;
            bullet[x] = 1000;
            bullet[y] = 1000;
            break; // Break out of the loop after processing the collision
        }
    }
    for (int i = 0; i < num; ++i) {

        if (bullet[x] >= mushroomarr[i][x] && bullet[x] <= mushroomarr[i][x] + boxPixelsX &&
            bullet[y] >= mushroomarr[i][y] && bullet[y] <= mushroomarr[i][y] + boxPixelsY && mushroombroke[i][exists] == false && mushroomarr[i][exists] == true) {
            mushroomarr[i][exists] = false;

            mushroombroke[i][exists] = true;
            bullet[exists] = false;
            bullet[x] = 1000; //bullet displacement 
            bullet[y] = 1000;//bullet displacement 
            break; // Break out of the loop after processing the collision
        }


        if (bullet[x] >= mushroomarr[i][x] && bullet[x] <= mushroomarr[i][x] + boxPixelsX &&
            bullet[y] >= mushroomarr[i][y] && bullet[y] <= mushroomarr[i][y] + boxPixelsY && mushroomarr[i][exists] == false && mushroombroke[i][exists] == true) {
            mushroombroke[i][exists] = false;
            mushroombroke[i][x] = 1000; //broken mushroom displaced 
            mushroombroke[i][y] = 1000; //broken mushroom displaced 
            score += 5;
            mushroomarr[i][exists] = false;
            mushroomarr[i][x] = 1000;  //normal mushroom displaced 
            mushroomarr[i][y] = 1000; //normal mushroom displaced 
            bullet[exists] = false;
            bullet[x] = 1000; //bullet displacement 
            bullet[y] = 1000;//bullet displacement 
            break; // Break out of the loop after processing the collision
        }
    }
    for (int i = num; i < num + 24; ++i) {

        if (bullet[x] >= mushroomarr[i][x] && bullet[x] <= mushroomarr[i][x] + boxPixelsX &&
            bullet[y] >= mushroomarr[i][y] && bullet[y] <= mushroomarr[i][y] + boxPixelsY && mushroombroke[i][exists] == false && mushroomarr[i][exists] == true) {
            mushroomarr[i][exists] = false;

            mushroombroke[i][exists] = true;

            bullet[exists] = false;
            bullet[x] = 1000; //bullet displacement 
            bullet[y] = 1000;//bullet displacement 
            break; // Break out of the loop after processing the collision
        }


        if (bullet[x] >= mushroomarr[i][x] && bullet[x] <= mushroomarr[i][x] + boxPixelsX &&
            bullet[y] >= mushroomarr[i][y] && bullet[y] <= mushroomarr[i][y] + boxPixelsY && mushroomarr[i][exists] == false && mushroombroke[i][exists] == true) {
            mushroombroke[i][exists] = false;
            mushroombroke[i][x] = 1000;  //broken mushroom displaced 
            mushroombroke[i][y] = 1000;  //broken mushroom displaced 
            score += 5;
            mushroomarr[i][exists] = false; //normal mushroom displaced 
            mushroomarr[i][x] = 1000;  //normal mushroom displaced 
            mushroomarr[i][y] = 1000;

            bullet[exists] = false;
            bullet[x] = 1000; //bullet displacement 
            bullet[y] = 1000;//bullet displacement 
            break; // Break out of the loop after processing the collision
        }
    }

}


void drawBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& bulletSprite) {
    bulletSprite.setPosition(bullet[x], bullet[y]);
    window.draw(bulletSprite);
}
void drawmashroom(sf::RenderWindow& window, float mushroomarr[][3], sf::Sprite& mushroom, int num, float mushroombroke[][3], sf::Sprite& broke, const int& player_area, const int& direction) {

    for (int i = 0; i < num + 24; i++) {


        if (mushroomarr[i][exists] == true && i < num)
        {
            mushroom.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
            //   broke.setTextureRect(sf::IntRect(32,0 , boxPixelsX, boxPixelsY));
            mushroom.setPosition(mushroomarr[i][x], mushroomarr[i][y]);
            window.draw(mushroom);
        }
        if (mushroombroke[i][exists] == true && i < num)
        {
            //  mushroom.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
            broke.setTextureRect(sf::IntRect(32, 0, boxPixelsX, boxPixelsY));
            broke.setPosition(mushroomarr[i][x], mushroomarr[i][y]);
            window.draw(broke);
        }

        if (mushroomarr[i][exists] == true && i >= num && mushroomarr[i][y] < (gameRows - 5) * boxPixelsY)
        {
            mushroom.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
            //broke.setTextureRect(sf::IntRect(32,0 , boxPixelsX, boxPixelsY));
            mushroom.setPosition(mushroomarr[i][x], mushroomarr[i][y]);
            window.draw(mushroom);
        }
        if (mushroombroke[i][exists] == true && i >= num && mushroombroke[i][y] < (gameRows - 5) * boxPixelsY)
        {
            //mushroom.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
            broke.setTextureRect(sf::IntRect(32, 0, boxPixelsX, boxPixelsY));
            broke.setPosition(mushroomarr[i][x], mushroomarr[i][y]);
            window.draw(broke);
        }

        if (mushroomarr[i][exists] == true && i >= num && mushroomarr[i][y] >= (gameRows - 5) * boxPixelsY)
        {
            mushroom.setTextureRect(sf::IntRect(0, 32, boxPixelsX, boxPixelsY));
            //broke.setTextureRect(sf::IntRect(32,0 , boxPixelsX, boxPixelsY));
            mushroom.setPosition(mushroomarr[i][x], mushroomarr[i][y]);
            window.draw(mushroom);
        }
        if (mushroombroke[i][exists] == true && i >= num && mushroombroke[i][y] >= (gameRows - 5) * boxPixelsY)
        {
            //mushroom.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
            broke.setTextureRect(sf::IntRect(32, 32, boxPixelsX, boxPixelsY));
            broke.setPosition(mushroomarr[i][x], mushroomarr[i][y]);
            window.draw(broke);
        }

    }
}

void setmushroom(float mushroomarr[][3], int num, float mushroombroke[][3], const int& player_area, const int& direction) {
    for (int i = 0; i < num + 24; i++) {
        if (i < num) {
            // Generated random coordinates for the mushroom within the game grid
            int j = rand() % gameColumns;
            int k = rand() % (gameRows - 7) + 1;

            // Set the coordinates and markedthe mushroom as existing
            mushroomarr[i][x] = j * boxPixelsX;
            mushroomarr[i][y] = k * boxPixelsY;
            mushroomarr[i][exists] = true;

            // Marked the corresponding "broken" mushroom as non-existing
            mushroombroke[i][exists] = false;
        }
        else {
            // Mark extra mushrooms as non-existing
            mushroomarr[i][exists] = false;
            mushroombroke[i][exists] = false;
        }
    }
}

void gameover(sf::RenderWindow& window, sf::Text& scoreText, sf::Font& font2) {
    // Created a clock to measure the delay time
    sf::Clock delayClock;

    // Defined the duration of the delay (adjust as needed)
    const sf::Time delayTime = sf::seconds(5.0f);

    // Set the "Game Over" text
    scoreText.setString("Game Over!  R.I.P");

    // Continued the loop while the window is open and the delay time is not reached
    while (window.isOpen() && delayClock.getElapsedTime() < delayTime) {
        // Handled events (e.g., window close event)
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }
        }

        // Cleared the window
        window.clear();

        // Loaded background texture and create a sprite
        sf::Texture backgroundTexture;
        sf::Sprite backgroundSprite;
        backgroundTexture.loadFromFile("Textures/Untitled Project(3).jpg");
        backgroundSprite.setTexture(backgroundTexture);

        // Set background sprite color and draw it
        backgroundSprite.setColor(sf::Color(255, 255, 255, 255 * 0.60));
        window.draw(backgroundSprite);

        // Set and draw the "Game Over" text
        scoreText.setString("Game Over!  R.I.P");
        window.draw(scoreText);

        // Displayed the window
        window.display();
    }

    // Closed the window or handle other actions when the game is over
    window.close();
}




