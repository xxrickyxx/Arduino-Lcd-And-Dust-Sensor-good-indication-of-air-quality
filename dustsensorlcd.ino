#include <TFT.h>  // Arduino LCD library
#include <SPI.h>
#include <time.h>

// pin definition for the Uno
#define cs   10
#define dc   9
#define rst  8
#define RED     0xF800

// create an instance of the library
TFT TFTscreen = TFT(cs, dc, rst);

int pin = 4;
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 2000; 
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;
void setup() {

Serial.begin(9600);
  pinMode(8,INPUT);
  pinMode(3, OUTPUT);
  starttime = millis(); 
  // Put this line at the beginning of every sketch that uses the GLCD:
  TFTscreen.begin();
  // clear the screen with a black background
  TFTscreen.background(0, 0, 0);

  // write the static text to the screen
  // set the font color to white
  TFTscreen.stroke(255, 255, 255);
  // set the font size
  TFTscreen.setTextSize(1);
  // write the text to the top left corner of the screen
  TFTscreen.text("Air Quality Box By\nRiccardo Sparacino ", 0, 0);
  // ste the font size very large for the loop
  TFTscreen.setTextSize(2);

}

void loop() {

 duration = pulseIn(pin, LOW);
  lowpulseoccupancy = lowpulseoccupancy+duration;
   TFTscreen.setTextWrap(false);
  TFTscreen.setCursor(20, 30);
 // TFTscreen.fillScreen(ST7735_GREEN);
  TFTscreen.setTextColor(ST7735_WHITE, ST7735_BLACK);
digitalWrite(3, HIGH);
  if ((millis()-starttime) >= sampletime_ms) //if the sampel time = = 30s
  {
    
    ratio = lowpulseoccupancy/(sampletime_ms*10.0);  
    concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; 
    lowpulseoccupancy = 0;
    starttime = millis();
    TFTscreen.setTextSize(1);
    TFTscreen.println("\n");
    TFTscreen.print(concentration);  
    TFTscreen.println(" pcs/0.01cf    ");
    TFTscreen.println("\n");
    TFTscreen.setTextColor(ST7735_RED, ST7735_BLACK);
           if  (concentration > 5000 and concentration < 1000){ TFTscreen.print("Dangerous Quality Air");
    }  if (concentration >= 10000) { TFTscreen.print("Very Dangerous Air  ");  
    }  if (concentration < 1000 and concentration > 10) { TFTscreen.print("Normal Quality Air   "); 
    }  if (concentration < 1) { TFTscreen.print("Good Quality Air     "); }
    
    printVolts();
    delay(100);
    

}  }
 void printVolts()
{
  int sensorValue = analogRead(A0); //read the A0 pin value
  float voltage = sensorValue * (3.70 / 1023.00) * 2; //convert the value to a true voltage.
  TFTscreen.setTextColor(ST7735_BLUE, ST7735_BLACK);
  TFTscreen.println("\n");
  TFTscreen.print("voltage = ");
  //TFTscreen.print(voltage); //print the voltage to LCD
  //TFTscreen.print(" V");
 
  
  TFTscreen.fill(0,0,255); // set the fill color to blue
  TFTscreen.stroke(255,255,255); // outline the rectangle with a white line
  TFTscreen.rect(60,85,30,10);
  TFTscreen.print("      ");
  TFTscreen.print(voltage);
  TFTscreen.print(" V");
  delay(100);
  if (voltage < 3.70) //set the voltage considered low battery here
  {
  TFTscreen.fill(RED); // set the fill color to blue
  TFTscreen.stroke(255,255,255); // outline the rectangle with a white line
  TFTscreen.rect(60,85,30,10);
  TFTscreen.print("      ");
  TFTscreen.print(voltage);
  TFTscreen.print(" V");
  delay(100);
  }

 // TFTscreen.println(" Battery Voltage");
}
