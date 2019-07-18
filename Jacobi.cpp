#include<cassert>
#include<limits> 
#include<cstddef>
#include<fstream>
#include<iostream> 
#include<iomanip>

typedef float Element_type;

class Matrix
{
    private:
	size_t mColCount;
	size_t mRowCount;
	Element_type * mData;
	
    public:
	void initialize( const std::size_t r, const std::size_t c )
	{
		mRowCount = r;
		mColCount = c;
		mData = new Element_type[r*c];
	}

	void setElement( const std::size_t i, const std::size_t j,const  Element_type  e)
	{
		mData[i* mColCount + j] = e;
	}
	
	Element_type & getElement( const std::size_t i , const std::size_t j) const
	{
		return mData[ i * mColCount + j ];
	}

	size_t getCol() const
	{
		return mColCount;
	}

	size_t getRow() const
	{
		return mRowCount;
	}
};

void readMatrix( Matrix & matrix, std::ifstream & like_pipe )
	{
	assert( like_pipe.is_open() );
	std::size_t r= std::numeric_limits<std::size_t>::max();
	like_pipe >> r;
	assert(r < std::numeric_limits<Element_type>::max() );
	assert(r > 0);
	assert( like_pipe.is_open() );
	std::size_t c = std::numeric_limits<std::size_t>::max();
	like_pipe >> c;
	assert(c < std::numeric_limits<Element_type>::max() );
	assert(c > 0);
	Element_type x = std::numeric_limits<Element_type>::max();
	matrix.initialize(r,c);
	for( std::size_t i = 0; i < r; ++i)
	{
		for( std::size_t j=0; j< c; ++j)
		{
			like_pipe>> x; //OK version
			//like_pipe.get();
	//		std::cout << x << " ";
			assert( x < std::numeric_limits<Element_type>::max());
			matrix.setElement(i,j,x);
		}
			std::cout << std::endl;
	}
}

Element_type myRound( Element_type var)
{
	Element_type value = (int)(var * 10000);
	return (Element_type)value/10000;
}


void writeMatrix(const Matrix & matrix , std::ofstream & like_pipe)
{
	assert(like_pipe.good());
	like_pipe<< matrix.getRow() << " ";
	assert(like_pipe.good());
	like_pipe<< matrix.getCol() << std::endl;

	for(std::size_t i = 0; i < matrix.getRow(); ++i)
	{
		for( std::size_t j = 0; j< matrix.getCol(); ++j)
		{
			like_pipe<< matrix.getElement(i,j); 
		}
		like_pipe << std::endl;
	}
}

void jacobiMethod(const  Matrix & a, const  Matrix &b, Matrix &x)
{
	std::size_t single_col= 0;
	x.initialize(a.getRow(), b.getCol()); 
	for( std::size_t i = 0; i<a.getRow(); ++i)
	{
		x.setElement(i,single_col,0);
	}
	std::size_t count = 0;
	while( count < 20)
	{
	for( std::size_t i= 0; i< a.getRow(); ++i) 
	{
		Element_type sum = 0;
		Element_type e = 0;
		assert( e < std::numeric_limits<Element_type>::max());	
		for( std::size_t j =0; j< a.getCol(); ++j)
		{
		 	if( i != j)
			{
				sum += x.getElement(i,single_col) * a.getElement(i,j);
			}
		}
				e = b.getElement(i,single_col) - sum;
				e =e/a.getElement(i,i); //
				e = myRound(e); 
				x.setElement(i, single_col, e);
		}
	 count++;
	}

}

int main()
{	
	Matrix a;
	Matrix b;
	Matrix x;
	std::ifstream A("A.txt"), B("B.txt");
	std::ofstream X("x.txt");
	readMatrix(a,A); 
	readMatrix(b,B);
	x.initialize(a.getRow() , b.getCol());
	jacobiMethod( a, b, x);
	writeMatrix(x, X);
	return 0;
}
