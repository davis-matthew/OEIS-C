/*
 * A106948  Primes of the form x^2+xy+18y^2, with x and y nonnegative.
 * https://oeis.org/A106948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 421, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   my_int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes of the form x^2 + x*y + 18*y^2 with x,y ≥ 0 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/my_int_sqrt.h"

/* 64‑bit deterministic Miller–Rabin primality test */
static bool is_prime64(uint64_t n){
    if(n < 2) return false;
    static const uint32_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47};
    for(size_t i=0;i<sizeof small/sizeof *small;i++){
        uint32_t p = small[i];
        if(n==p) return true;
        if(n%p==0) return false;
    }
    uint64_t d=n-1,s=0;
    while((d&1)==0){d>>=1; s++;}
    uint64_t bases[]={2,3,5,7,11,13,17};
    for(size_t i=0;i<sizeof bases/sizeof *bases;i++){
        uint64_t a = bases[i]%n;
        if(a==0) continue;
        __uint128_t x=1, base=a, exp=d;
        while(exp){
            if(exp&1) x=(x*base)%n;
            base=(base*base)%n;
            exp>>=1;
        }
        if(x==1||x==n-1) continue;
        bool composite=true;
        for(uint64_t r=1;r<s;r++){
            x=(x*x)%n;
            if(x==n-1){ composite=false; break;}
        }
        if(composite) return false;
    }
    return true;
}

/* comparison function for qsort on seq_t */
static int cmp_seq(const void *a,const void *b){
    const seq_t *u=(const seq_t*)a;
    const seq_t *v=(const seq_t*)b;
    if(*u<*v) return -1;
    if(*u>*v) return 1;
    return 0;
}

seq_t a(idx_t n){
    if(n==0) return (seq_t)0;          /* offset starts at 1 */

    /* static state across calls */
    static seq_t *primes=NULL;
    static size_t prime_cap=0, prime_size=0;
    static idx_t max_val=0;

    if(primes==NULL){
        prime_cap=256;
        primes=(seq_t*)malloc(prime_cap*sizeof(seq_t));
        if(!primes) abort();
    }

    while(prime_size < n){
        /* increase search bound */
        idx_t new_bound = (max_val==0) ? 1000 : max_val*2 + 1000;

        /* generate values of the quadratic form in (max_val, new_bound] */
        idx_t y_max = my_int_sqrt(new_bound/18);
        for(idx_t y=0; y<=y_max; ++y){
            __uint128_t ypart = (__uint128_t)18*y*y;
            idx_t x=0;
            for(;;++x){
                __uint128_t val128 = (__uint128_t)x*x + (__uint128_t)x*y + ypart;
                if(val128>new_bound) break;
                if(val128<=max_val) continue;
                if(val128 > UINT64_MAX) continue;     /* cannot test >64‑bit */
                uint64_t val64 = (uint64_t)val128;
                if(is_prime64(val64)){
                    seq_t val = (seq_t)val64;
                    if(prime_size>=prime_cap){
                        prime_cap*=2;
                        seq_t *tmp=(seq_t*)realloc(primes,prime_cap*sizeof(seq_t));
                        if(!tmp) abort();
                        primes=tmp;
                    }
                    primes[prime_size++]=val;
                }
            }
        }
        max_val = new_bound;

        /* sort and deduplicate the list of primes found so far */
        if(prime_size>1) qsort(primes,prime_size,sizeof(seq_t),cmp_seq);
        size_t uniq=0;
        for(size_t i=0;i<prime_size;i++){
            if(uniq==0 || cmp_seq(&primes[uniq-1],&primes[i])!=0){
                primes[uniq++]=primes[i];
            }
        }
        prime_size=uniq;
    }

    return primes[n-1];
}
