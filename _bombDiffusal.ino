//Globals
//pins
int relayPin = 9;

int threeWayKeySwitch1Pin = 2;
int threeWayKeySwitch2Pin = 3;

int greenLEDPin = 13;
int redLEDPin = 12;
int yellowLEDPin = 8;

// second part of the two way key switch
int setButtonPin = 11;
// cable plug in part
int cablePlugInPin = A0;

//keys
int key1Mom = 4;
int key2Mom = A4; // 5
int key3 = A5; // 6
int key4AllWrong = 7;

//Pots and knob pins
int voltageMeterTop = 5;
int voltageMeterBottom = 6;

int potKnobTop = A1;
int potKnobBottom = A2;

int meterDeviationPlusMinus = 20;
int one = 64;
int four = 218;

int resetButtonPin = 10;

//Game Logic Vars
int isGreen = 0;
int isRed = 0;

int isKey1 = 0;
int isKey2 = 0;
int isKey3 = 0;
int isKey4 = 0;

int setPressed = 0;
int state = 1;

// FUNCS
void onSuccess(){
  digitalWrite(relayPin, HIGH);
  delay(3000);
  digitalWrite(relayPin, LOW);
}

void blinkAll() {
  for (int i = 0; i <= 10; i++) {
    digitalWrite(greenLEDPin, HIGH);
    digitalWrite(redLEDPin, HIGH);
    digitalWrite(yellowLEDPin, HIGH);
    delay(150);
    digitalWrite(greenLEDPin, LOW);
    digitalWrite(redLEDPin, LOW);
    digitalWrite(yellowLEDPin, LOW);
    delay(150);
  }
}

void blinkGreen() {
  for (int i = 0; i <= 10; i++) {
    digitalWrite(greenLEDPin, HIGH);
    delay(150);
    digitalWrite(greenLEDPin, LOW);
    delay(150);
  }
}

void blinkRed() {
  for (int i = 0; i <= 10; i++) {
    digitalWrite(redLEDPin, HIGH);
    delay(150);
    digitalWrite(redLEDPin, LOW);
    delay(150);
  }
}
void yellowBlink() {
  for (int i = 0; i <= 10; i++) {
    digitalWrite(yellowLEDPin, HIGH);
    delay(150);
    digitalWrite(yellowLEDPin, LOW);
    delay(150);
  }
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(relayPin, OUTPUT);
  pinMode(threeWayKeySwitch1Pin, INPUT_PULLUP);
  pinMode(threeWayKeySwitch2Pin, INPUT_PULLUP);
  pinMode(setButtonPin, INPUT_PULLUP);
  pinMode(cablePlugInPin, INPUT_PULLUP);
  pinMode(key1Mom, INPUT_PULLUP);
  pinMode(key2Mom, INPUT_PULLUP);
  pinMode(key3, INPUT_PULLUP);
  pinMode(key4AllWrong, INPUT_PULLUP);

  pinMode(resetButtonPin, INPUT_PULLUP);
  
  //Pot setup
  pinMode(potKnobTop, INPUT);
  pinMode(potKnobBottom, INPUT);

  pinMode(voltageMeterTop, OUTPUT);
  pinMode(voltageMeterBottom, OUTPUT);

  digitalWrite(voltageMeterTop, HIGH);
  digitalWrite(voltageMeterBottom, LOW);
  

  //LED setup
  pinMode(greenLEDPin, OUTPUT);
  pinMode(redLEDPin, OUTPUT);
  pinMode(yellowLEDPin, OUTPUT);

  digitalWrite(greenLEDPin, LOW);
  digitalWrite(redLEDPin, LOW);
  digitalWrite(yellowLEDPin, LOW);

  digitalWrite(relayPin, LOW);

}

void loop() {
  SM_01();
}


void SM_01() {
//STATE MACHINE GLOBALS
  int threeWayKeySwitch1;
  int threeWayKeySwitch2;
  int setButton;
  int resetButton = digitalRead(resetButtonPin);
  int cablesPluggedIn;

  int keyOneMom;
  int keyTwoMom;
  int keyThree;
  int keyFourWrong;

  keyOneMom = digitalRead(key1Mom);
  keyTwoMom = digitalRead(key2Mom);
  keyThree = digitalRead(key3);
  keyFourWrong = digitalRead(key4AllWrong);

  int outputVal1;
  int outputVal2;
     
  if(resetButton){
    //reset Globals
    blinkAll();
    state = 0;
  }
  
  switch (state) {
    case 0: // reset
    Serial.println("Puzzle has been reset");
      // reset all globals
      isGreen = 0;
      isRed = 0;
      state = 0;
      setPressed = 0;
      Serial.println("IN SM STATE_OF_PUZZLE IS 0, GOING TO 1");
      state = 1;
      break;
    case 1: // Check to see if the LED is green or Red. If green and button press State progresses to 2.
      Serial.println("You are at State 1");
      threeWayKeySwitch1 = digitalRead(threeWayKeySwitch1Pin);
      threeWayKeySwitch2 = digitalRead(threeWayKeySwitch2Pin);
      setButton = digitalRead(setButtonPin);

      if (!threeWayKeySwitch1) {
        digitalWrite(greenLEDPin, HIGH);
        digitalWrite(redLEDPin, LOW);
        isGreen = 1;
        isRed = 0;
      } else if (!threeWayKeySwitch2) {
        digitalWrite(redLEDPin, HIGH);
        digitalWrite(greenLEDPin, LOW);
        isGreen = 0;
        isRed = 1;
      } else {
        digitalWrite(redLEDPin, LOW);
        digitalWrite(greenLEDPin, LOW);
        isGreen = 0;
        isRed = 0;
      }

      if (isGreen && !setButton) {
        state = 2;
        blinkGreen();
      }
      break;

      case 2:
      //The Single Key is done now for cable orientations
      cablesPluggedIn = digitalRead(cablePlugInPin);
      
      Serial.println("You are at State 2");
      if(!cablesPluggedIn){
        yellowBlink();
        state = 3;
      }
      
      break;

      case 3:
      //Serial.println("You are at State 3");
      digitalWrite(redLEDPin, HIGH);
      digitalWrite(yellowLEDPin, HIGH);

      
      outputVal1 = map(analogRead(potKnobTop), 0, 1023, 0, 255);
      outputVal2 = map(analogRead(potKnobBottom), 0, 1023, 0, 255);
      
      analogWrite(voltageMeterBottom, outputVal1);
      analogWrite(voltageMeterTop, outputVal2);

      Serial.println(outputVal1);
      Serial.println(outputVal2);


        if(((outputVal1 < (four + meterDeviationPlusMinus)) && (outputVal1 > (four - meterDeviationPlusMinus)) && (outputVal2 < (one + meterDeviationPlusMinus)) && (outputVal2 > (one - meterDeviationPlusMinus)) && !digitalRead(setButtonPin))){
          Serial.println("YOU WIN VOLTS1!");
  
          digitalWrite(yellowLEDPin, LOW);
          digitalWrite(greenLEDPin, HIGH);
          delay(100);
          digitalWrite(greenLEDPin, LOW);
          digitalWrite(redLEDPin, LOW);
          delay(100);
          digitalWrite(redLEDPin, HIGH);
          digitalWrite(yellowLEDPin, HIGH);
          delay(100);
          digitalWrite(yellowLEDPin, LOW);
          digitalWrite(greenLEDPin, HIGH);
          delay(100);
          digitalWrite(greenLEDPin, LOW);
          digitalWrite(redLEDPin, LOW);
          delay(100);
          digitalWrite(redLEDPin, HIGH);
          digitalWrite(yellowLEDPin, HIGH);
          delay(100);
          digitalWrite(yellowLEDPin, LOW);
          digitalWrite(greenLEDPin, HIGH);
          delay(100);
          digitalWrite(greenLEDPin, LOW);
          digitalWrite(redLEDPin, LOW);
          delay(100);
          digitalWrite(redLEDPin, HIGH);
          digitalWrite(yellowLEDPin, HIGH);
          delay(100);
          digitalWrite(yellowLEDPin, LOW);
          digitalWrite(redLEDPin, LOW);
          state = 4;
        }
      break;
      
      case 4:
      Serial.println("You are at State 4");
      threeWayKeySwitch1 = digitalRead(threeWayKeySwitch1Pin);
      threeWayKeySwitch2 = digitalRead(threeWayKeySwitch2Pin);
      setButton = digitalRead(setButtonPin);
      
      if (!threeWayKeySwitch1) {
        digitalWrite(greenLEDPin, HIGH);
        digitalWrite(redLEDPin, LOW);
        isGreen = 1;
        isRed = 0;
      } else if (!threeWayKeySwitch2) {
        digitalWrite(redLEDPin, HIGH);
        digitalWrite(greenLEDPin, LOW);
        isGreen = 0;
        isRed = 1;
      } else {
        digitalWrite(redLEDPin, LOW);
        digitalWrite(greenLEDPin, LOW);
        isGreen = 0;
        isRed = 0;
      }

      if (isRed && !setButton) {
        //Serial.println(setPressed);
        setPressed++;
        delay(50);
        if(setPressed > 175){
          blinkRed();
          state = 5;
        }
      } 
      
      break;
      
   
      
      case 5:
      Serial.println("YOU GOT TO SIX");
        threeWayKeySwitch1 = digitalRead(threeWayKeySwitch1Pin);
        threeWayKeySwitch2 = digitalRead(threeWayKeySwitch2Pin);
        
        keyOneMom = digitalRead(key1Mom);
        keyTwoMom = digitalRead(key2Mom);
        keyThree = digitalRead(key3);
        keyFourWrong = digitalRead(key4AllWrong);
        
      if (!keyOneMom) {
        isKey1 = 1;
        Serial.println("KEY 1");
      } else {
        isKey1 = 0;
        //Serial.println("KEY 1 OFF");
      } 
      
      if (!keyTwoMom) {
        isKey2 = 1;
        Serial.println("KEY 2");
      } else {
        isKey2 = 0;
        //Serial.println("KEY 2 OFF");
      } 

      if (!keyThree) {
        isKey3 = 1;
        Serial.println("KEY 3");
      } else {
        isKey3 = 0;
        //Serial.println("KEY 3 OFF");
      } 

      if (!keyFourWrong) {
        isKey1 = 0;
        isKey2 = 0;
        isKey3 = 0;
        isKey4 = 1;
      } else {
        isKey4 = 0;
      }

      if (!threeWayKeySwitch1) {
        digitalWrite(greenLEDPin, HIGH);
        digitalWrite(redLEDPin, LOW);
        isGreen = 1;
        isRed = 0;
      } else if (!threeWayKeySwitch2) {
        digitalWrite(redLEDPin, HIGH);
        digitalWrite(greenLEDPin, LOW);
        isGreen = 0;
        isRed = 1;
      } else {
        digitalWrite(redLEDPin, LOW);
        digitalWrite(greenLEDPin, LOW);
        isGreen = 0;
        isRed = 0;
      }
       
      if(isKey1 && isKey2 && isKey3 && !isKey4 && isGreen){
         Serial.println("YOU GOT ALL THE KEYS!!!!");
         state = 6;
      }

      break;
      
      case 6:
      blinkAll();
      state = 7;
      break;
      
      case 7:
      onSuccess();
      state = 0;
      break;

    default:
      //Serial.println("Something went wrong.");
      delay(2000);
      state = 0;
      break;
  }
}
