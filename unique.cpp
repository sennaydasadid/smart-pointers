#include <iostream>
#include <memory>
#include <utility>

using namespace std;

//creamos un funcion que utilice parametros "unique_ptr"
void funcion_( unique_ptr < int > ptr ){
    *ptr =*ptr + 2;
    cout<<"\nContenido de ptr "<<*ptr;
    cout<<"\nEjemplo de puntero inteligente "<<endl;
}
//creamos un funcion que devuelve unique_ptr
unique_ptr < int > foo (){
    unique_ptr <int> ptr=make_unique <int> ( 59);
    return ptr ;
}

class Rectangulo { 
    int largo; 
    int ancho; 
  
public: 
    Rectangulo(int l, int b){ 
        largo = l; 
        ancho = b; 
    } 
  
    int area() { 
        return largo * ancho; 
    } 
}; 
template<class Arr>
struct shared_array_maker {};
template<class T, std::size_t N>
struct shared_array_maker<T[N]> {
    std::shared_ptr<T> operator()const{
        auto r = std::make_shared<std::array<T,N>>();
        if (!r) return {};
        return {r.data(), r};
    }
};
template<class Arr>
auto make_shared_array()
-> decltype( shared_array_maker<Arr>{}() )
{ return shared_array_maker<Arr>{}(); }


struct Good: std::enable_shared_from_this<Good>{
    std::shared_ptr<Good> getptr() {
        return shared_from_this();
    }
};
struct Bad {
    std::shared_ptr<Bad> getptr() {
        return std::shared_ptr<Bad>(this);
    }
    ~Bad() { std::cout << "Bad::~Bad() called\n"; }
};
 


int main () {
    
    //---------unique_ptr - make_unique------
    unique_ptr<int> ptr=make_unique<int>(15); //creamos un puntero inteligente
    *ptr=1;//agregamos contenido
    unique_ptr<int>ptr2=move(ptr);//movemos contenido

    funcion_( move ( ptr2));

    unique_ptr <int > ptr3 = foo () ;
    cout<<"Contenido de ptr"<<*ptr3<<endl;

    //creamos un arreglo dinamico con "auto"
    //arrPtr es como una matriz 
    auto arrPtr=std::make_unique<int[]>(10);
    arrPtr[0]=1;
    arrPtr[1]=5;
    arrPtr[2]=10;//se modifoca el elemento que esta en la poscicion 2.
    cout<<arrPtr[1]<<endl;


    //---------shared_ptr-------------------
    //P1 apunta al objeto Rectangulo
    shared_ptr<Rectangulo> P1(new Rectangulo(10, 5)); 
    //P2 tambien apunta al objeto Rectangulo
    shared_ptr<Rectangulo> P2; 
    P2 = P1; 
    //P1 y P2 tienen los mismos valores
    cout << P2->area() << endl; 
    cout << P1->area() << endl; 
    //obtenemos el número de referencia
    cout << P1.use_count() << endl;

    //creamos un arreglo 
    shared_ptr <int> sharr(new int [10], default_delete<int[]>());//C++11
    shared_array_maker<int [10]>;
    shared_ptr<int[]>sh(new int [10]);//C++17

    // Transfiriendo la propiedad
    shared_ptr <int> up = make_shared <int> (12) ;
    shared_ptr <int> up2 = move(up);
    // En este punto, el recuento de referencia de up=0 
    // y up2 con referencia count = 1


    //---------weak_ptr--------
    std::shared_ptr<int> sp (new int);
    std::weak_ptr<int> wp1;
    std::weak_ptr<int> wp2 (sp);

    std::cout << "use_count:\n";
    std::cout << "wp1: " << wp1.use_count() << '\n';
    std::cout << "wp2: " << wp2.use_count() << '\n';



    // Good: los dos  shared_ptr comparten el mismo objeto
    std::shared_ptr<Good> gp1(new Good);
    std::shared_ptr<Good> gp2 = gp1->getptr();
    std::cout << "gp2.use_count() = " << gp2.use_count() << '\n';
 
    // Bad, cada shared_ptr cree que es el unico dueño del objeto
    std::shared_ptr<Bad> bp1(new Bad);
    std::shared_ptr<Bad> bp2 = bp1->getptr();
    std::cout << "bp2.use_count() = " << bp2.use_count() << '\n';
   

}