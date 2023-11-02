//Khai bao dau vao            
int in1 = 8;
int in2 = 9;
int in3 = 10;
int in4 = 11;
                                    
unsigned char STEP_ARRAY[8] = {0x01,0x05,0x04,0x06,0x02,0x0A,0x08,0x09};  

//-------------------//
void setup()
{
  Serial.begin(9600);      
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(2, OUTPUT); //chan reset
  digitalWrite(2, HIGH); //cho chan reser len muc cao
}



//-------- fullstep 1 pha ON -----------//                 
void fullstep(int rpm, int ppr, boolean dir)
{   
  if(dir == 1)
  { 
    for(int i = 0; i < ppr/4; i++)   // 0   2    4    6
    {                         
      for(int j = 0; j < 8; j++)
        {
          PORTB = STEP_ARRAY[j++];   
          delayMicroseconds(300000/rpm);
        }
    }
  }
    else 
    {                              
      for(int i = ppr/4; i > 0; i--)   // 6   4   2   0
       {
        for(int j = 6; j >= 0; j--)
        {
          PORTB = STEP_ARRAY[j--];
          delayMicroseconds(300000/rpm);
        }
      }
   }
}
//-------- fullstep 2 pha ON -----------// 
void fullstep_2pha_ON(int rpm, int ppr, boolean dir)
{   
  if(dir == 1){                           
    for(int i = 0; i < ppr/4; i++)
    {
      for(int j = 1; j < 8; j++)
        {
          PORTB = STEP_ARRAY[j++];
          delayMicroseconds(300000/rpm);
        }
      }
    
    }
    else 
    {                              
      for(int i = ppr/4; i > 0; i--)
      {
         for(int j = 7; j > 0; j--)
          {
            PORTB = STEP_ARRAY[j--];
            delayMicroseconds(300000/rpm);
          }
      }
   }
}

//-------halfstep -------//    
void halfstep(int rpm, int ppr, boolean dir)
{                          
  if(dir == 1)
   {
    for(int i = 0; i < ppr; i++)
      {
        PORTB = STEP_ARRAY[i%8];
        delayMicroseconds(150000/rpm);
      }
    }
  else 
   {
    for(int i = ppr; i > 0; i--)
      {
        PORTB = STEP_ARRAY[i%8];
        delayMicroseconds(150000/rpm); 
      }
    }
}


//----------------//
void DK_STEP_MOTOR()
{
int i=0;
int mode; //loai dieu khien
float X ; //set khoang cach
boolean dir1;  //set chieu quay
int rpm1, A;  //set van toc
int ppr1;  //set so xung can di chuyen 1 doan X

  Serial.print("Hay nhap khoang cach di chuyen (mm): ");
  do 
  {
    X = Serial.parseFloat();  //gán giá trị cho X
  }
  while (X == 0);  //X chua duoc nhap
  Serial.println(X);  
  Serial.print("Hay nhap toc do (vong / phut): ");
  
  do 
  {  
    rpm1 = Serial.parseInt();  //gán giá trị cho rpm1
  }
  while (rpm1 == 0);
  Serial.println(rpm1);
  Serial.print("Hay nhap che do dieu khien (1: Full - step - 1 pha ON, 2: Half - step, 3: fullstep - 2pha - ON : ");
  
  do 
  {  
    mode = Serial.parseInt();
  }
  while (mode == 0);
  A = X/abs(X);   //chon chieu duong, am
  Serial.println(mode);

  switch(A)
  {
          case 1: 
                  
                  dir1 = 1;   
                  
                  break;
          case -1: 
                  dir1 = 0;   
                  
                  break;
  }
  ppr1 = (abs(X)/8)*200;  //xung phai di trong 1 vong
  switch(mode)
    {
          case 1: 
                  //delay(3000);
                  fullstep(rpm1, ppr1, dir1);   // 130 < RPM < 420          
                  
                  break;
          case 2: 
                  //delay(3000);
                  halfstep(rpm1, 2*ppr1, dir1);  // 133 < RPM < 500     
                  
                  break;
                         
          case 3:
                   fullstep_2pha_ON(rpm1, ppr1, dir1);   //  
                  break;
    }
}

void loop()
{
DK_STEP_MOTOR(); 
digitalWrite(2, LOW);
}
