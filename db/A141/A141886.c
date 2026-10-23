/*
 * A141886  Primes congruent to 17 mod 20.
 * https://oeis.org/A141886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) gives the n-th prime congruent to 17 mod 20
 * Range:     u8 n <= 6, u16 n <= 825, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* <utils> functions used: is_prime(idx_t), allocate_seq_array,
   free */ 
#include "../utils/is_prime.h"

extern bool is_prime(idx_t);

/* Sequence function */
seq_t a(idx_t n)
{
    /* static cache of primes of the form 20k + 17 */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_size = 0;
    static idx_t last_candidate = 17; /* next candidate to test */
    static bool initialized = false;

    /* allocate or grow cache if needed */
    if (!initialized) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_size = 0;
        initialized = true;
    } else if (cache_cap < n) {
        size_t new_cap = cache_cap;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* memory error */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* generate further primes until cache_size >= n */
    while (cache_size < n) {
        /* test current candidate */
        if (is_prime(last_candidate)) {
            cache[cache_size++] = (seq_t)last_candidate;
        }
        last_candidate += 20; /* next number that is 17 mod 20 */
    }

    return cache[n - 1];
}
