/*
 * A158201  Primes with sum of largest digit and final digit is a prime.
 * https://oeis.org/A158201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, less
 * Formula:   a(n) = the n-th prime p such that the sum of the largest decimal
 *            digit of p and the final digit of p is itself a prime. The
 *            sequence starts at a(1)=11
 * Range:     u8 n <= 8, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Helper: test if num (small, ≤ 18) is prime. */
static inline bool is_small_prime(unsigned int num) {
    switch(num){
        case 2: case 3: case 5: case 7: case 11: case 13: case 17: return true;
        default: return false;
    }
}

/* ----------------- Prime storage and trial division ----------------- */
static seq_t *primes = NULL;
static size_t primes_len = 0;
static size_t primes_cap = 0;

static void primelist_resize(size_t newcap) {
    if(newcap <= primes_cap) return;
    size_t cap = primes_cap? primes_cap : 8;
    while(cap < newcap) cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(primes, cap * sizeof(seq_t));
    if(!tmp){ /* OOM */ abort(); }
    primes = tmp;
    primes_cap = cap;
}

static void primelist_add(seq_t p){
    primelist_resize(primes_len + 1);
    primes[primes_len++] = p;
}

/* Trial division using the primes list already collected. */
static bool primetest(seq_t n){
    if(n < 2) return false;
    for(size_t i = 0; i < primes_len; ++i){
        seq_t p = primes[i];
        if(p > n / p) break;          /* avoid overflow */
        if(n % p == 0) return false;
    }
    return true;
}

/* ------------------- Sequence cache -------------------------------- */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t next_candidate = 2;  /* start checking from 2 */

/* Ensure cache has at least n entries. */
static void ensure_cache(idx_t n){
    if(n <= cache_len) return;
    /* Grow cache array. */
    size_t needed = cache_len + (size_t)n;
    if(needed > cache_cap){
        size_t cap = cache_cap? cache_cap : 8;
        while(cap < needed) cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        if(!tmp){ abort(); }
        cache = tmp;
        cache_cap = cap;
    }
    /* Find further qualifying primes. */
    while(cache_len < (size_t)n){
        seq_t curr = next_candidate;
        /* Test primality. */
        if(primetest(curr)){
            /* Prime found, add to prime list. */
            primelist_add(curr);
            /* Compute last digit and largest digit. */
            unsigned int last_digit = (unsigned int)(curr % 10);
            unsigned int max_digit = last_digit;
            seq_t temp = curr / 10;
            while(temp){
                unsigned int d = (unsigned int)(temp % 10);
                if(d > max_digit) max_digit = d;
                temp /= 10;
            }
            unsigned int sum = last_digit + max_digit;
            if(is_small_prime(sum)){
                cache[cache_len++] = curr;
            }
        }
        next_candidate++;
    }
}

/* ------------------- Public function -------------------------------- */
seq_t a(idx_t n)
{
    if(n < 1) return 0;          /* undefined, but avoid UB */
    ensure_cache(n);
    return cache[n-1];
}
