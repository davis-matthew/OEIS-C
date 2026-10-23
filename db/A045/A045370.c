/*
 * A045370  Primes congruent to {0, 2, 4, 6} mod 7.
 * https://oeis.org/A045370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p such that p mod 7 ∈ {0,2,4,6}
 * Range:     u8 n <= 27, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"       /* Prototypes for is_prime() */

/* Static cache for computed terms */
static seq_t *cache = NULL;      /* Dynamic array holding the terms */
static size_t cache_cap = 0;     /* Current capacity of the array */
static idx_t  cache_len = 0;     /* Number of terms stored */
static idx_t  next_candidate = 2; /* Next integer to test for primality */

/* Helper to grow the cache when needed */
static void grow_cache(void)
{
    size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) { /* allocation failed */
        exit(EXIT_FAILURE);
    }
    cache     = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. Behaviour for n == 0 is undefined. */
    while (cache_len < n) {
        if (cache_len == cache_cap) {
            grow_cache();
        }

        /* Test the next candidate for primality */
        if (is_prime(next_candidate)) {
            idx_t mod7 = next_candidate % 7;
            if (mod7 == 0 || mod7 == 2 || mod7 == 4 || mod7 == 6) {
                cache[cache_len] = (seq_t)next_candidate;
                ++cache_len;
            }
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
