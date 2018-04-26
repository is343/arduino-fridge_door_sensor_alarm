/* 
Refrigirator door alarm -- Ivan Schurawel 
original idea from: Adithya Pai from instructables
*/
#include <avr/sleep.h>

#define sleepInterrupt 2
#define doorSensor 8  
#define alert 13 //Buzzer and LED, pin 11 for variable voltage
#define doorOpenIndicator 10 //Open door LED, pin 10 for variable voltage


int closedSeconds = 0;
int openSeconds = 0;


void setup()
{
  pinMode(sleepInterrupt, INPUT);
  pinMode(doorSensor, INPUT);
  pinMode(alert, OUTPUT);
  pinMode(doorOpenIndicator, OUTPUT);
}


void loop()
{
  delay(1000); // second intervals
  
  if(digitalRead(doorSensor) == HIGH) // door closed
  {
    closedSeconds++;
    openSeconds = 0;
    digitalWrite(doorOpenIndicator,LOW);
  } 
  
  if(digitalRead(doorSensor) == LOW) // door open
  {
    closedSeconds = 0;
    openSeconds++;
    digitalWrite(doorOpenIndicator,HIGH);
    //analogWrite(doorOpenIndicator,15); // for variable brightness up to 255
  }

  if(closedSeconds >= 160) // sleep if closed for awhile
  {
    closedSeconds = 0;
    enterSleep();
  }
  
  if((openSeconds >=120)&&(digitalRead(doorSensor)==LOW)){
    
    while(digitalRead(doorSensor) == LOW)
    {
      closedSeconds = 0; // reset everything
      openSeconds = 0;
      digitalWrite(doorOpenIndicator,LOW);
      //analogWrite(alert,1); // for variable volume and brightness up to 255
      digitalWrite(alert,HIGH);
      delay(300);
      digitalWrite(doorOpenIndicator,HIGH);
      digitalWrite(alert,LOW);
      delay(300);
      digitalWrite(doorOpenIndicator,LOW);
    }
    enterSleep(); // door closed, so enter sleep immediately
    
  }
  
}

void enterSleep() // sleep to save energy
{
  attachInterrupt(0, exitSleep, LOW); // 0 === pin2, calls fxn to exit sleep, when LOW
  delay(100);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);//define power down sleep mode
  sleep_cpu(); // set sleep enable (SE) bit, this puts the cpu to sleep
}

void exitSleep() // called to exit sleep
{
  sleep_disable(); // clears the SE bit, always do before detaching the interrupt
  detachInterrupt(0); // disable the interrupt
}
