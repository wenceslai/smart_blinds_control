#include <IRremote.h>
#include <Dusk2Dawn.h>
#include <virtuabotixRTC.h>
#include <Stepper.h>


#define led 13
virtuabotixRTC myRTC(6, 7, 8);

int day, month, year, minutes_since_midnight;
int sunrise, sunset;
bool daylight_saving;
bool is_night;
bool blinds_open = true;
bool manual_blinds_open = true;

const int RECV_PIN = 12;
IRrecv irrecv(RECV_PIN);
decode_results results;
unsigned long key_value = 0;


Dusk2Dawn location(49.8257, 18.2675, 260); // Ostrava latitude, longitude, elevation


const int stepsPerRevolution = 200;
Stepper myStepper = Stepper(stepsPerRevolution, 8, 9, 10, 11);



void setup() {
  Serial.begin(9600);

  // secs, minutes, hours, day of the week, day of the month, month ,year
  //myRTC.setDS1302Time(00, 48, 9, 3, 28, 4, 2021);

  irrecv.enableIRIn();
  
  pinMode(led, OUTPUT);

  myStepper.setSpeed(70);
}

void loop() {
  myRTC.updateTime();

  day = myRTC.dayofmonth;
  month = myRTC.month;
  year = myRTC.year;
  /*
  minutes_since_midnight = myRTC.hours * 60 + myRTC.minutes; 

  // on and off state for automatic mode

  if((month > 3 and month < 10) or (month == 3 and day >= 28) or (month == 10 and day <= 31)){
    daylight_saving = true;
   } else {
    daylight_saving = false;
   }
  
  sunrise = location.sunrise(year, month, day, daylight_saving);
  sunset = location.sunset(year, month, day, daylight_saving);

  if (minutes_since_midnight < sunrise or minutes_since_midnight > sunset) {
      is_night = true;
  } else if (minutes_since_midnight > sunrise or minutes_since_midnight < sunset)  {
      is_night = false;  
  }

  // chaning blinds state if in transition state

  if(minutes_since_midnight == sunrise && blinds_open == false){
      //open_blinds();
      blinds_open == true;
  } else if(minutes_since_midnight == sunset && blinds_open == true) {
      //close_blinds();
      blinds_open = false;  
  }
  */
  // AUTOMATIC MODE
 
  if (irrecv.decode(&results)){
        digitalWrite(led,HIGH);
        if (results.value == 0XFFFFFFFF)
          results.value = key_value;

        switch(results.value){
          
          case 0xFF22DD:  
            Serial.println("|<< - CLOSING BLINDS");   

            if(blinds_open == true){
              close_blinds();
              blinds_open = false;
            
            } else {
              Serial.println("ERROR BLINDS ALREADY CLOSED");
            }
            
          break;
          
          case 0xFF02FD: 
            Serial.println(">>| OPENING BLINDS");

            if(blinds_open == false){
              open_blinds();
              blinds_open = true;
              
            } else {
              Serial.println("ERROR BLINDS ALREADY OPENED");
            }
          break ;  
          /*
          case 0xFFC23D:
          Serial.println(">|");
          break ;               
          case 0xFFE01F:
          Serial.println("-");
          break ;  
          case 0xFFA857:
          Serial.println("+");
          break ;  
          case 0xFF906F:
          Serial.println("EQ");
          */
        }
        key_value = results.value;
        irrecv.resume();

        digitalWrite(led,LOW);
  }

}

void open_blinds(){
 
  
  myStepper.step(120);
  delay(1000);
}

void close_blinds(){
  

  myStepper.step(-120);
  delay(1000);
}



  
