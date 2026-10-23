/*
 * A137772  Prime numbers p such that p +- ((p-1)/30030) are primes.
 * https://oeis.org/A137772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 8, u128 n <= 8, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A137772: prime p such that p ± ((p-1)/30030) are primes. */
/* The sequence lists all primes p with that property (offset = 1). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

/* Cache for already found terms of the sequence. */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;

/* Current k (the multiplier) used to generate candidates p = 30030*k + 1. */
static idx_t next_k = 1;

/* Grow the cache when needed. */
static void grow_cache(void)
{
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else {
        size_t new_cap = cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp) {
            cache = tmp;
            cache_cap = new_cap;
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Generate terms until the cache has at least n elements. */
    while (cache_size < (size_t)n) {
        seq_t k = (seq_t)next_k;
        seq_t p  = k * (seq_t)30030 + (seq_t)1;   /* 30030*k + 1 */
        seq_t q1 = k * (seq_t)30029 + (seq_t)1;   /* 30029*k + 1 = p - k */
        seq_t q2 = k * (seq_t)30031 + (seq_t)1;   /* 30031*k + 1 = p + k */

        if (is_prime_big(p) && is_prime_big(q1) && is_prime_big(q2)) {
            if (cache_size == cache_cap) grow_cache();
            cache[cache_size++] = p;
        }
        ++next_k;
    }
    return cache[n - 1];
}
