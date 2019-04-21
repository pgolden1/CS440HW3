#ifndef SHAREDPTR_H
#define SHAREDPTR_H

#include <cstdlib>
#include <mutex>

namespace cs540{



template <typename T>
class SharedPtr{ 
	private:
		T* dataptr;
		static unsigned int refCount;
		std::mutex objLock;
	public:
	
	
		//=================Constructors, Assignment Operators, Destructor =================
		SharedPtr(){
			dataptr = nullptr;
			refCount = 0;
				
		}
		
		template <typename U> explicit SharedPtr(U* objptr){
			dataptr = (T*) objptr;
			refCount = 1;
		}
		
	
		SharedPtr(const SharedPtr &p){
			if(p.get() != nullptr){
				dataptr = (T*) p.get();
				std::lock_guard<std::mutex> objL(objLock);
				refCount++;
				p.increment(); // check if you actually have to double increment
			}
		}
	
		template <typename U> SharedPtr(SharedPtr<U> &p){
			if(p.get() != nullptr){
				dataptr = (T*) p.get();
				std::lock_guard<std::mutex> objL(objLock);
				refCount++;
				p.increment(); // check if you actually have to double increment
			}
		}
		
	
		SharedPtr (SharedPtr &&p);
	
		template <typename U> SharedPtr(SharedPtr<U> &&p);
		
	
		SharedPtr &operator=(const SharedPtr &p);

		template <typename U> SharedPtr<T> &operator=(const SharedPtr<U> &p);
		
		
		SharedPtr &operator=(SharedPtr &&p);

		template <typename U> SharedPtr &operator=(SharedPtr<U> &&p);
		
		
		~SharedPtr(){
			std::lock_guard<std::mutex> objL(objLock);
			if(--refCount == 0){
				//delete dataptr;
				dataptr = nullptr;
			}

		}
		
		
		
		
		//================= Modifiers =================
		
		void reset() {
			dataptr = nullptr;
			refCount--;
		}
		
		template <typename U> void reset(U *p){
			std::lock_guard<std::mutex> objL(objLock);
			if(--refCount == 0){
				//delete dataptr;
				dataptr = (T*) p;
				refCount = 1;
			}
		
		}
		
		
		
		
		//================= Observers =================
		
		T *get() const{
			return dataptr;
		}
		
		T &operator*() const{
			return *dataptr;
		}
		
		T *operator->() const{
			return dataptr;
		}
		
		explicit operator bool() const{
			return (dataptr != nullptr);
		}
		
		void increment(){
			std::lock_guard<std::mutex> objL(objLock);
			refCount++;
		}
		
		void decrement(){
			std::lock_guard<std::mutex> objL(objLock);
			if(--refCount == 0){
				//delete dataptr;
				dataptr = nullptr;
			}
		}

};


//================= Free standing functions =================

template <typename T1, typename T2>
bool operator==(const SharedPtr<T1> &a, const SharedPtr<T2> &b){
	if(a.get() == b.get()) return true;
	if(a.get() == nullptr && b.get() == nullptr) return true;
	return false;
}


template <typename T>
bool operator==(const SharedPtr<T> &a, std::nullptr_t b){
	if(a.get() == nullptr) return true;
	return false;
}

template <typename T>
bool operator==(std::nullptr_t b, const SharedPtr<T> &a){
	if(a.get() == nullptr) return true;
	return false;
}

template <typename T1, typename T2>
bool operator!=(const SharedPtr<T1> &a, const SharedPtr<T2> &b){
	return !(a == b);
}

template <typename T>
bool operator!=(const SharedPtr<T> &a, std::nullptr_t b){
	return !(a == b);
}

template <typename T>
bool operator!=(std::nullptr_t b, const SharedPtr<T> &a){
	return !(a == b);
}

template <typename T, typename U>
SharedPtr<T> static_pointer_cast(const SharedPtr<U> &sp);

template <typename T, typename U>
SharedPtr<T> dynamic_pointer_cast(const SharedPtr<U> &sp);

}


#endif
