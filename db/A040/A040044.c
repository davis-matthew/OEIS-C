/*
 * A040044  Primes p such that x^3 = 5 has a solution mod p.
 * https://oeis.org/A040044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p for which the congruence x^3 ≡ 5 (mod p) has a solution */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"

static bool qualifies(uint64_t p){
    if(p==2 || p==3) return true;
    if(p%3==2) return true;
    seq_t res = mod_pow((seq_t)5, (p-1)/3, (seq_t)p);
    return res == (seq_t)1;
}

static uint64_t *prime_list = NULL;
static size_t prime_list_len = 0;
static size_t prime_list_cap = 0;
static uint64_t candidate = 3;

static uint64_t next_prime(void){
    if(prime_list_len==0){
        prime_list_cap = 8;
        prime_list = (uint64_t*)realloc(NULL, prime_list_cap * sizeof(uint64_t));
        prime_list[0] = 2;
        prime_list_len = 1;
        return 2;
    }
    while(1){
        bool isprime = true;
        for(size_t i=0;i<prime_list_len;i++){
            uint64_t p = prime_list[i];
            if(p*p > candidate) break;
            if(candidate % p == 0){ isprime = false; break; }
        }
        if(isprime){
            if(prime_list_len == prime_list_cap){
                prime_list_cap <<= 1;
                prime_list = (uint64_t*)realloc(prime_list, prime_list_cap * sizeof(uint64_t));
            }
            prime_list[prime_list_len++] = candidate;
            uint64_t ret = candidate;
            candidate += 2;
            return ret;
        }
        candidate += 2;
    }
}

seq_t a(idx_t n){
    if(n==0) return 0;
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    size_t target = (size_t)n;
    while(cache_len < target){
        uint64_t p = next_prime();
        if(qualifies(p)){
            if(cache_len == cache_cap){
                cache_cap = cache_cap? cache_cap*2 : 8;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)p;
        }
    }
    return cache[target-1];
}
