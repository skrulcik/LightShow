/*
  Analog input, analog output, serial output
 
 Reads an analog input pin, maps the result to a range from 0 to 255
 and uses the result to set the pulsewidth modulation (PWM) of an output pin.
 Also prints the results to the serial monitor.
 
 The circuit:
 * potentiometer connected to analog pin 0.
   Center pin of the potentiometer goes to the analog pin.
   side pins of the potentiometer go to +5V and ground
 * LED connected from digital pin 9 to ground
 
 created 29 Dec. 2008
 modified 9 Apr 2012
 by Tom Igoe
 
 This example code is in the public domain.
 
 #Girl, sleep sound by Jamie XX
 #Juno Reactor Bible of Dreams
 for the winner by molly ringwald
 UNenglish by international music 
 */
#define ORIGINAL 0
#define COL_SWAP 1
#define RED_PIN 9
#define GREEN_PIN 10
#define BLUE_PIN 11

// These constants won't change.  They're used to give names
// to the pins used:
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int sense_radius = 30;
const int out_pins[] = {RED_PIN, GREEN_PIN, BLUE_PIN};
const int cycle_limit = 7;
int cycle_count = 0;
int pin_seed = 0; //0-2, represents value of live_pin
int mode = ORIGINAL;

uint8_t live_pin;
uint8_t avg_pin;
uint8_t wave_pin;

int sensorValues[sense_radius];        // value read from the pot
//int live_out = 0;
int avg_out = 0;
int wave_out = 0;


void cyclePins(){
  pin_seed = (pin_seed+1) % 3;
  live_pin = out_pins[pin_seed];
  avg_pin = out_pins[(pin_seed+1) % 3];
  wave_pin = out_pins[(pin_seed+2) % 3];
  //delay();
}

void randomValues(){
  analogWrite(live_pin, random(0, 4));
  analogWrite(avg_pin, random(0, 12));
  analogWrite(wave_pin, random(7, 52));
}

void setup() {
  cyclePins();
}

void write_live(uint8_t val)
{
  analogWrite(live_pin, val);
}

void write_avg(int current_avg)
{
  analogWrite(avg_pin, (current_avg)/4);
}

void write_wave()
{
  wave_out = (wave_out+10) % 250;
  if(wave_out > 125){
    analogWrite(wave_pin, (250-wave_out)/70+1);
  }else{
    analogWrite(wave_pin, wave_out/70+1);
  }
}

void loop() {
  switch(mode){
    case ORIGINAL:
      int sum = 0;
      for(int i=0; i<sense_radius; i++){
        sensorValues[i] = min(510, (analogRead(analogInPin)*15))/2;
        if (sensorValues[i] >= 250){
          cyclePins();
        }
        sum += sensorValues[i];
        write_live(sensorValues[i]);
        delay(2);
      }
      write_avg(sum/sense_radius); //Takes average of both last reads and current
      write_wave();     
      if(wave_out >= 230){
        cycle_count += 1;
      }
      break;
    case COL_SWAP:
     int sense = min(510, (analogRead(analogInPin)*15))/2;
     if (sense >= 200){
          cyclePins();
          randomValues();
     }
     delay(1);
     break;
  }
}
