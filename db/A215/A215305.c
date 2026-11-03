/*
 * A215305  Primes congruent to {1, 2, 3, 4} mod 19.
 * https://oeis.org/A215305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p such that p % 19 ∈ {1,2,3,4}
 * Range:     u8 n <= 15, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 65 terms through n = 981 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Use helper functions for allocating an array of seq_t. */

seq_t a(idx_t n)
{
    /* static cache storing already found primes */
    static seq_t *cache  = NULL;   /* array of primes found so far   */
    static size_t cache_cap = 0;   /* capacity of the array          */
    static size_t cache_used = 0;  /* number of elements stored      */
    static idx_t  next_candidate = 2; /* next integer to test for primality */

    /* Initialize cache once */
    if (cache == NULL) {
        cache_cap = 8;
        cache = seq_alloc(cache_cap);   /* allocates an array of seq_t with zero init */
    }

    /* Extend cache until it contains at least n elements */
    while (cache_used < n) {
        /* Ensure there is space for another prime */
        if (cache_used == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Test the current candidate for being the next qualifying prime */
        if (is_prime(next_candidate)) {
            idx_t r = next_candidate % 19;
            if (r == 1 || r == 2 || r == 3 || r == 4) {
                /* Store it in the cache */
                cache[cache_used++] = (seq_t)next_candidate;
            }
        }
        /* Move to the next integer for testing */
        ++next_candidate;
    }

    return cache[n - 1];
}
