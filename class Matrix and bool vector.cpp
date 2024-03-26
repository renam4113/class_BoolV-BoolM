#include <iostream>
#include <string.h>
#include <fstream>

using namespace std;


// Класс булев вектор
class BoolV
{ unsigned int *v;
 int nbit, m; // nbit – количество бит вектора, m – количество элементов массива v
 public:
 BoolV(int nn = 1); // формируется булев вектор из nn бит; все биты со значениями 0
 BoolV(const char*); // формируется булев вектор по строке из нулей и единиц
 BoolV( BoolV &);
 ~BoolV(){if(v) delete []v; v = NULL;};
 void Set1(int);
 void Set0(int);
 void clear_vec();
 unsigned int& operator[](int);
 BoolV& operator = (const BoolV &);
 bool operator == (const  BoolV &); // равенство векторов
 BoolV operator | (const BoolV &);
 BoolV operator & (const BoolV &);
 BoolV operator ~ ();
 friend ostream& operator << (ostream &,  BoolV &);
 friend istream& operator >> (istream &, BoolV &); 
 int weight(); 
 BoolV operator << (int); 
 BoolV operator >> (int); 
 friend class BoolM;
};


 // сделать для булевых векторов разной длины


 

 
 BoolV BoolV:: operator & (const BoolV & vec){
    int z = nbit;
    if(nbit > vec.nbit) z = vec.nbit; 
    BoolV A(z);
    for(int i = 0; i < m; i++){
      A.v[i] = v[i] & vec.v[i];
    }
    return A;
 }






// void inversion(unsigned char *vec, size_t bits){ 

// int k = bits % 8; 
// unsigned char m = (1 « k) - 1; 
// int bytes = ((bits - 1) / 8) + 1; 
// for (int i = 0; i < bytes ; i++){ 
// vec[i] = ~vec[i]; 
// } 
// *vec = *vec & m; 
// } 
  // подправить 
  BoolV BoolV:: operator ~ (){
    int k = nbit % 32;
    unsigned char mask = 1;
    mask = mask >> 31 - 1;
    for(int i = 0; i < m; i++) v[i] = ~v[i];
    return *this;

  }


  istream& operator >> (istream & in, BoolV & vec){
    char A[500];
    in >> A;
    BoolV vecA(A);
    vec = vecA;
    return in;
  }



  ostream& operator << (ostream & out,  BoolV & vec){
    for(int i = 0; i < vec.m; i++){
        unsigned int mask = 1;
        int j = 0;
        if(i == 0) j = 32 - vec.nbit % 32;
        mask = mask  << (31 - j);
        for(; j < 32; j++){
            if(vec.v[i] & mask) out << '1';
            else out << '0';
            mask = mask >> 1;
        }
        out << "  ";
    }
    return out;
  }

  BoolV BoolV:: operator << (int k){
    unsigned int bite1 = 0;
    unsigned int bite2 = 0;
    unsigned int mask = (1 << k) - 1;
    mask = mask << (32 - k);

    for( int i = m - 1; i >= 0 ; i--){
        bite1 = v[i] & mask;
        v[i] = v[i] << k;
        v[i] = v[i] | bite2 >> (32 - k);
        bite2 = bite1;
    }
    return (*this);
  }

  BoolV BoolV:: operator >> (int k){
    unsigned int bite1 = 0;
    unsigned int bite2 = 0;
    unsigned int mask = (1 << k) - 1;

    for(int i = 0; i < m; i++){
        bite1 = v[i] & mask;
        v[i] = v[i] >> k;
        v[i] = v[i] | bite2 << (32 - k);
        bite2 = bite1;
    }
    return (*this);
  }
 

 BoolV& BoolV:: operator = (const BoolV &vec){
    if(this == &vec) return (*this);
    nbit = vec.nbit;
    m = vec.m;
    delete []v;
    v = new unsigned int[m];
    for(int i = 0; i < m; i++) v[i] = vec.v[i];
    return (*this);
 }

 int BoolV:: weight(){
    int count = 1;
    for(int i = 0; i < m; i++){
        unsigned int mask = 1;
        mask = mask << 31;
        for(int j = 0; j < 32; j++){
            if(v[i] & mask != 0) count++;
            mask = mask >> 1;
        }
    }
    return count;
 }


 bool BoolV:: operator == (const BoolV & vec){
    if(m != vec.m ) return false;
    for(int i = 0; i < m; i++) if(v[i] != vec.v[i]) return false;
    return true;
 }


 unsigned int& BoolV:: operator[](int i){
     if (i < 0 || i >= m){
        cout << "неправильный индекс";
         return v[0];
     }
    
     return v[i];

 }


 void BoolV:: Set0(int k){
    unsigned int mask = 1;
    int shift = ((k - 1) / 32 + 1) * 32 - k;
     mask = (mask << (32 - shift)) ;
    v[m - (k / 32) - 1] ^= mask;
 }

 void BoolV:: Set1(int k){
    unsigned int mask = 1;
    int shift = ((k - 1) / 32 + 1) * 32 - k;
     mask = mask << (32 - shift);
    v[m - (k / 32) - 1] |= mask;
 }

 BoolV:: BoolV( BoolV &vec){
    nbit = vec.nbit;
    m = vec.m;
    v = new unsigned int[m];
    for(int i = 0; i < m; i++) v[i] = vec.v[i];
 }



 BoolV:: BoolV(const char* vec){
    nbit = strlen(vec);
    m = ((nbit - 1) / 32) + 1;
    v = new unsigned int[m];
    for(int i = 0; i < m; i++) v[i] = 0;
    for(int i = 0; i < m; i++){
        unsigned int mask = 1;
        int j = 0;
        if(i == 0) j = m * 32 - nbit;
        mask = mask << (31 - j);
        
        for(; j < 32; j++){
            if(*vec == '1') v[i] = v[i] | mask;
            mask = mask >> 1;
            vec++;
        }
    }
}
BoolV:: BoolV(int nn){
    nbit = nn;
    m = ((nn - 1) / 32) + 1;
    v = new unsigned int[m];
    for(int i = 0; i < m; i++) v[i] = 0;
 }

 BoolV BoolV:: operator | (const BoolV & vec){
    if(m < vec.m){
      int k = 0;
      BoolV A(vec.nbit);
      for(int s = vec.m; s > m; s--){
          A.v[k] |= vec.v[k];
          k++;
      }
      for(int i = 0; i < m; i++){
        A.v[i] = v[i] | vec.v[k];
        k++;
      }
      return A;
    }
    else{
      BoolV A(nbit);
      int k = 0;
      for(int s = vec.m; s > m; s--){
         A.v[k] |= v[k];
         k++;
      }
      for(int i = 0; i < vec.m; i++){
        A.v[i] = v[k] | vec.v[i];
        k++;
      }
      return A;
    }
 }



class BoolM
{ BoolV *bm;
 int m, n; // m – количество строк, n – количество столбцов
 public:
 BoolM(int k = 1, int l = 1); // формируется нулевая булева матрица размера kl
 BoolM(const BoolM &);

 BoolV & operator[] (int);
 BoolM operator = (const BoolM &);
 bool topsort(int*);
 friend ostream & operator << (ostream &, BoolM &);
};

BoolV & BoolM:: operator[](int k){
  if(k >= m || k < 0) {
    cout << "неправильный индекс" << endl;
    return bm[0];
  }
  return bm[k];
} 

BoolM BoolM:: operator = (const BoolM & mas){
  if(this == &mas) return (*this);
  if(bm) delete []bm;
  n = mas.n;
  m = mas.m;
  bm = new BoolV[m];
  for(int i = 0; i < m; i++) bm[i] = mas.bm[i];
  return (*this);
}

// спросить 
// ostream & operator <<(ostream & out, BoolM & matrix){
//   for(int i = 0; i < matrix.m; i++){
//     out << matrix.bm[i] << endl;
//   }

//   return out;
// }

BoolM:: BoolM(const BoolM & matrix){
  n = matrix.n;
  m = matrix.m;
  bm = new BoolV[m];
  for(int i = 0; i < m; i++) bm[i] = matrix.bm[i];
}

BoolM:: BoolM(int k , int l){
  m = k;
  n = l;
  bm = new BoolV[m];
  for(int  i = 0; i < m; i++){
    BoolV vec(n);
    bm[i] = vec;
  }
}

ostream& operator << (ostream & out, BoolM & mas){
  for(int i = 0; i < mas.m; i++) out << mas.bm[i] << endl;
  return out;
}



//Топологическая сортировка на булевой матрице
bool BoolM:: topsort(int *a){
  bool flag = true;
  int k = 0;
  int count = 0;
  BoolV v0(m);
  int arr[m] = {0};
  while(flag){
    if (count == m) return true;
    for(int i = 0; i < m; i++) v0 = v0 | bm[i];
    BoolV mask(m);
    mask.Set1(m - 1);
    flag = false;
    for(int i = 0; i < m; i++){
      if(!(mask.v[0] & v0.v[0]) && arr[i] == 0){
        flag = true;
        a[k] = i + 1;
        k++;
        bm[i].v[0] = 0;
        arr[i] = 1;
        count++;
      }
      mask = mask >> 1;
    }
    if(!flag){
      cout << "matrix have cycle" << endl;
      return false;
    }
    BoolV mak(m);
    v0 = v0 & mak;
  }
}



int main(){
  BoolM mas;
  char A[56] = "1010110011010101101011001010101111001010010101101010101";
  BoolV Vec(A);
   //cin >> vec;
  //cout << vec << endl;
  // cout << Vec << endl; 
  // //BoolV vecA = vec | Vec;
  // //BoolV vecB = vec & Vec;
  // //cout << vecA << endl;
  // //cout << vecB << endl;
  // Vec << 6;
  // cout << Vec << endl;
  // ~Vec;
  // cout << Vec << endl;
  //cout << ~vecA;
  ifstream in("graf.txt"); 
  int k;  
  if (in.is_open()){
      in >> k;
      BoolM Mas(k, k);
      mas = Mas;
      int a, b;
       while(in >> a >> b){ 
          mas[a - 1].Set1(k - b);
       }
  }
  
  in.close(); 
  cout << Vec << endl;
  BoolV vec = ~Vec;
  cout << vec << endl;
   cout <<  mas << endl;
  int* a = new int[k];

  // if(mas.topsort(a)){
  //   cout << "ответ : ";
  //   for(int i = 0; i < k; i++) cout << a[i] << " ";
  // }
  return 0;
}