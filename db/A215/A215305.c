/*
 * A215305  Primes congruent to {1, 2, 3, 4} mod 19.
 * https://oeis.org/A215305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p such that p % 19 ∈ {1,2,3,4}
 * Range:     u8 n <= 15, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t x); */

/* Use helper functions for allocating an array of seq_t. */
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

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
