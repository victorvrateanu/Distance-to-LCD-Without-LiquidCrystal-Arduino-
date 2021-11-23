#define D4 5
#define D5 4
#define D6 3
#define D7 2
#define RS 12   // 0 - command   1 - data 
#define E 11    // low - read, high- write
#define TRIG 10 
#define ECHO 9
#define V0 6

// byte = unsigned int 0-255
void LCD_Write(byte cmd, int mod) {
    digitalWrite(RS, mod); //Change RS for command/data
    // this method generates latency, but saves 4 data pins for other uses

    digitalWrite(D4, (cmd & 0b00010000) >> 4);
    digitalWrite(D5, (cmd & 0b00100000) >> 5);
    digitalWrite(D6, (cmd & 0b01000000) >> 6);
    digitalWrite(D7, (cmd & 0b10000000) >> 7);
    delay(0.5);
    digitalWrite(E, HIGH);
    delay(0.5);
    digitalWrite(E, LOW);
    delay(0.5);
    digitalWrite(D4, (cmd & 0b00000001) >> 0);
    digitalWrite(D5, (cmd & 0b00000010) >> 1);
    digitalWrite(D6, (cmd & 0b00000100) >> 2);
    digitalWrite(D7, (cmd & 0b00001000) >> 3);
    delay(0.5);
    digitalWrite(E, HIGH);
    delay(0.5);
    digitalWrite(E, LOW);
    delay(0.5);
}


void LCD_Char(char character) {
    LCD_Write(character, 1);
}

void LCD_Clear() {
    LCD_Write(0x01, 0);
}

void LCD_Init() {
    LCD_Write(0x33, 0); // ready the 8 bits
    delay(0.5);
    LCD_Write(0x32, 0); // 8 bits
    delay(0.5);
    LCD_Write(0x28, 0); // 4 bits, 2 lines
    delay(0.5);
    LCD_Write(0x0C, 0); // display-on, cursor- off
    delay(0.5);
    LCD_Write(0x06, 0); // auto-increment cursor and disable shift mod
    delay(0.5);
    LCD_Write(0x01, 0); // display clean-up
    delay(0.5);
}

void LCD_SetCursor(int row, int col) {
    if (col == 0) {
        LCD_Write((row | 0x80), 0);
    }

    delay(0.5);

}

void LCD_Int(float num) {

    char c[6];
    int digit = 0;
    dtostrf(num, 6, 8, c);
    do
    {
        num = num / 10;
        digit++;
    } while (num != NULL);


    for (int i = 0; i < 6; i++) {
        LCD_Char(c[i]);
    }

    /*

    char c[10];
    int digit=0;

    do
    {
      c[digit] = (char)(num % 10) + '0';
      num=num/10;
      digit++;
    }while(num!=NULL);
    for(int i=digit-1; i >= 0;i--){
        LCD_Char(c[i]);
    }
    */

}
int contrast = 50;
float distance;
float time;

void setup()
{
    Serial.begin(9600);
    for (int i = 2; i < 13; i++)
    {
        pinMode(i, OUTPUT);
    }
    analogWrite(6, contrast);
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);
    delay(50);
    LCD_Init();
}

void loop() {
    LCD_SetCursor(0, 0);
    LCD_Char('C');
    LCD_Char('M');
    LCD_Char(':');

    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);

    time = pulseIn(ECHO, HIGH);

    distance = time * 0.034 / 2;
    Serial.println(distance);
    LCD_SetCursor(0, 4);
    LCD_Int(distance);

}