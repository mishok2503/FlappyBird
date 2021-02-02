#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <fstream>

using namespace sf;

RenderWindow window(VideoMode(1200, 900), "Flappy Bird");

int power(int a, int b)
{
    if (b != 0)
    {
        int c = a;
        for (int i=1; i < b; i++) a *= c;
    }
    else a = 1;
    return a;
}

class Bird
{
private:

    struct Pos {float x, y;};
    Pos pos, p;
    int image_hight, image_up, image_len, im_up,
    im_l, im_d, die_move;
    float fall_speed, fall_a, time;
    bool IsLife, IsJump, first_die;
    Texture bird_te;
    Sprite bird_im;
    Image bird_image;
    Clock clock;

public:
    Bird()
    {
        first_die = true;
        IsLife = true;
        IsJump = false;
        pos.x = 100;
        pos.y = 500;
        fall_speed = 0.01f;
        fall_a = 0.0125;
        clock.restart();
    }

    void set_image(int color)
    {
        switch (color)
        {
        case 1:
            bird_te.loadFromFile("images/red_bird.png");
            bird_im.setScale(0.5, 0.5);
            image_hight = 70;
            image_up = -18;
            image_len = 75;
            im_up = 30;
            im_l = 150;
            die_move = 50;
            im_d = 40;
            break;
        case 2:
            bird_image.loadFromFile("images/blue_bird.png");
            bird_image.createMaskFromColor(Color::White);
            bird_te.loadFromImage(bird_image);
            bird_im.setPosition(pos.x, pos.y);
            bird_im.setScale(0.6, 0.6);
            image_hight = 83;
            image_up = -25;
            image_len = 80;
            im_up = 40;
            im_l = 145;
            die_move = 50;
            im_d = 40;
            break;
        case 3:
            bird_te.loadFromFile("images/yel_red_bird.png");
            bird_im.setScale(0.12, 0.12);
            image_hight = 135;
            image_up = -70;
            image_len = 103;
            im_up = 87;
            im_l = 150;
            im_d = 42;
            die_move = 85;
            break;
        case 4:
            bird_image.loadFromFile("images/yellow_bird.png");
            bird_image.createMaskFromColor(Color::White);
            bird_te.loadFromImage(bird_image);
            bird_im.setScale(0.12, 0.12);
            image_hight = 70;
            image_up = -8;
            image_len = 80;
            im_up = 28;
            im_l = 150;
            im_d = 28;
            die_move = 32;
            break;
        }
        bird_im.setTexture(bird_te);
        bird_im.setPosition(pos.x, pos.y);
    }

    void set_IsJump() {IsJump = true;}
    void restart() {clock.restart();}
    void die() {IsLife = false;}
    bool get_IsLife() {return IsLife;}
    int get_im_left() {return im_l;}
    int get_im_down() {return im_d;}
    int get_d() {return image_hight + pos.y;}

    Pos get_pos()
    {
        p = pos;
        p.x += image_len;
        p.y += im_up;
        p.y = window.getSize().y - p.y;
        return p;
    }

    void _move_(bool is_some_life)
    {
        time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time = time / 2850;

        if (pos.y > window.getSize().y - image_hight && IsLife) IsLife = false;
        if (IsLife)
        {
            if (pos.y < image_up) {pos.y = image_up; IsJump = false;}
            if (IsJump)
            {
                fall_speed = -1.8;
                IsJump = false;
                pos.y += fall_speed * time;
            }
            else
            {
                fall_speed += fall_a * time;
                pos.y += fall_speed * time;
            }
        }
        else
        {
            if (first_die)
            {
                bird_im.setRotation(40);
                first_die = false;
                pos.x += die_move;
            }
            if (pos.y < window.getSize().y - image_hight)
            {
                fall_speed += fall_a * time;
                pos.y += fall_speed * time * 2;
            }
            if (pos.x + image_len + 10 > 0 && is_some_life) pos.x -= 0.5 * time;
        }
    }

    void draw()
    {
        bird_im.setPosition(pos.x, pos.y);
        window.draw(bird_im);
    }

};

class Barier
{
private:
    Texture te;
    Sprite bar_down, bar_up;
    struct Pos {float x, y;};
    Pos pos;
    float time;
    Clock clock;
public:

    Barier()
    {
        te.loadFromFile("images/barier.png");
        bar_down.setTexture(te);
        bar_down.setTextureRect(IntRect(576, 0, 245, 1500));
        bar_up.setTextureRect(IntRect(576, 0, 245, 1500));
        bar_up.setTexture(te);
        bar_up.setScale(0.4, -0.6);
        bar_down.setScale(0.4, 0.6);
    }

    void _move_(bool is_some_life)
    {
        if (is_some_life)
        {
            time = clock.getElapsedTime().asMicroseconds();
            clock.restart();
            time = time / 3150;
            pos.x -= 0.5 * time;
        }
    }

    void draw()
    {
        bar_down.setPosition(pos.x, window.getSize().y - pos.y);
        bar_up.setPosition(pos.x, window.getSize().y - pos.y - 200);
        window.draw(bar_down);
        window.draw(bar_up);
    }

    void set_pos(float x, float y)
    {
        pos.x = x;
        pos.y = y;
    }

    int get_x_pos() {return pos.x;}
    Pos get_pos() {return pos;}
};

class Net
{
private:

    double x[5];

public:

    Net()
    {
        for (int i=0; i < 5; i++) x[i] = (rand() % 200) / 100 - 1;
    }

    Net(const Net& n)
    {
        for (int i=0; i < 5; i++) x[i] = n.x[i];
    }

    void update(Int64 score)
    {
        for (int i=0; i < 5; i++) x[i] += ((rand() % 30) - 15) / (score / 300);
    }

    bool net_res(double up_bar, double down_bar, double len_bar, double up, double down)
    {
        bool result = false;
        double res = 0;

        res += x[0] * up_bar;
        res += x[1] * down_bar;
        res += x[2] * len_bar;
        res += x[3] * up;
        res += x[4] * down;

        if (res > 10) result = true;

        return result;
    }

};

const int GENERATION_SIZE = 50;

Bird bird[GENERATION_SIZE];
Barier barier[5];

int main()
{
    int life_bird_1, life_bird_2;
    Net net[GENERATION_SIZE];
    bool IsSomeLife = true;
    int score = 0, current_bar = 0;
    Int64 net_score = 0;
    Texture bg_te, num_te;
    num_te.loadFromFile("images/numbers.png");
    Sprite bg, num[4];
    bg_te.loadFromFile("images/bg.png");
    bg.setTexture(bg_te);
    bg.setScale(1.35, 1.8);
    srand(time(0));
    barier[0].set_pos(850, rand() % 550 + 100);
    for (int i=1; i < 5; i++) barier[i].set_pos(barier[i - 1].get_x_pos() + 400, rand() % 550 + 100);

    for (int i=0; i < GENERATION_SIZE; i++) bird[i].set_image(rand() % 4 + 1);

    for (int i=0; i < 4; i++)
    {
        num[i].setScale(1.5, 1.5);
        num[i].setTexture(num_te);
        num[i].setPosition(430 + (4 - i)*75, 0);
    }

    bool not_pressed = true, IsNewScore = true;
    for (int i=0; i < GENERATION_SIZE; i++) bird[i].restart();
	while (window.isOpen())
	{
	    net_score = 0;
	    while (IsSomeLife  && window.isOpen())
        {
            net_score++;

            if (score == 1000)
            {
                window.close();
                for (int i=0; i < 10; i++) std::cout<<"win!!!";
            }

            Event event;
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                    window.close();
                if (event.type == Event::KeyReleased) not_pressed = true;
            }
            for (int i=0; i < 4; i++)
            {
                if (int((score % power(10, i + 1)) / power(10, i)) == 0) num[i].setTextureRect(IntRect(380, 0, 40, 60));
                else num[i].setTextureRect(IntRect(((int((score % power(10, i + 1)) / power(10, i)) - 1) * 41.3) + 10, 0, 40, 60));
            }

            window.clear();
            window.draw(bg);
            for (int i=0; i < 5; i++) barier[i].draw();
            for (int i=0; i < GENERATION_SIZE; i++) bird[i].draw();
            for (int i=0; i < 4; i++) window.draw(num[i]);
            window.display();

            IsSomeLife = false;
            for (int i=0; i < GENERATION_SIZE; i++)
            {
                if (bird[i].get_IsLife()) IsSomeLife = true;
                life_bird_1 = i;
            }
            for (int i=0; i < life_bird_1; i++) if (bird[i].get_IsLife()) life_bird_2 = i;

            for (int j=0; j < GENERATION_SIZE; j++)
            {
                if (net[j].net_res(barier[current_bar].get_pos().y + 190 - bird[j].get_pos().y, bird[j].get_pos().y - barier[current_bar].get_pos().y, barier[current_bar].get_pos().x - bird[j].get_pos().x, window.getSize().y - bird[j].get_pos().y, window.getSize().y - bird[j].get_d()))
                    bird[j].set_IsJump();

                bird[j]._move_(IsSomeLife);
                for (int i=0; i< 5; i++)
                {
                    if (bird[j].get_pos().x > barier[i].get_pos().x && bird[j].get_pos().x < barier[i].get_pos().x + bird[j].get_im_left())
                        if (bird[j].get_pos().y < barier[i].get_pos().y + bird[j].get_im_down() || bird[j].get_pos().y > barier[i].get_pos().y + 190) bird[j].die();
                }
            }

            for (int j=0; j < GENERATION_SIZE; j++)
            {
                if (!bird[j].get_IsLife()) continue;
                bool IsBreak = false;
                for (int i=0; i< 5; i++)
                {
                    if (bird[j].get_pos().x > barier[i].get_pos().x + bird[j].get_im_left() + 8 && IsNewScore)
                    {
                        score++;
                        current_bar++;
                        current_bar %= 5;
                        IsNewScore = false;
                        IsBreak = true;
                        break;
                    }
                }
                if (IsBreak) break;
            }

            for (int i=0; i < 5; i++)
            {
                barier[i]._move_(IsSomeLife);
                if (barier[0].get_x_pos() + 100 < 0) {barier[0].set_pos(barier[4].get_x_pos() + 400,  rand() % 600 + 100); IsNewScore = true; break;}
                if (i != 0)
                    if (barier[i].get_x_pos() + 100 < 0) {barier[i].set_pos(barier[i - 1].get_x_pos() + 400, rand() % 600 + 100); IsNewScore = true; break;}
            }
        }
        Sleep(200);
        for (int i=0; i < GENERATION_SIZE / 2; i++)
        {
            if (i == life_bird_2) continue;
            net[i] = net[life_bird_1];
        }
        for (int i= GENERATION_SIZE / 2; i < GENERATION_SIZE; i++)
            net[i] = net[life_bird_2];
        for (int i=0; i < GENERATION_SIZE; i++)
        {
            bird[i] = Bird();
            bird[i].set_image(rand() % 4 + 1);
            if (i != GENERATION_SIZE - 2) net[i].update(net_score);
        }
        score = 0;
        barier[0].set_pos(850, rand() % 550 + 100);
        for (int i=1; i < 5; i++) barier[i].set_pos(barier[i - 1].get_x_pos() + 400, rand() % 550 + 100);
        IsSomeLife = true;
        current_bar = 0;
    }

	return 0;
}
