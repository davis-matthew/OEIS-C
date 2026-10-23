/*
 * A045319  Primes congruent to {1, 2, 3, 4} (mod 5).
 * https://oeis.org/A045319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045319: primes congruent to 1,2,3,4 (mod 5)  
   (i.e., all primes except 5).  a(1)=2, a(2)=3, a(3)=7, … */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"

/* Static cache of sequence values.
   cache[0] holds a(1). */
static seq_t *cache = NULL;          /* array of known values */
static size_t cache_len = 0;         /* number of stored values */
static size_t cache_cap = 0;         /* allocated capacity */
static idx_t   next_candidate = 2;   /* next integer to test for primality */

/* Ensure that cache contains at least n terms. */
static void
ensure_n(idx_t n)
{
    while (cache_len < n) {
        /* Grow the cache if necessary. */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_arr) abort();         /* out‑of‑memory – exit the program */
            cache = new_arr;
            cache_cap = new_cap;
        }

        /* Skip prime number 5. */
        if (next_candidate != 5 && is_prime_small((seq_t)next_candidate)) {
            cache[cache_len++] = (seq_t)next_candidate;
        }
        next_candidate++;  /* advance to the next candidate */
    }
}

seq_t
a(idx_t n)
{
    if (n == 0)          /* OEIS offset is 1; behaviour for n<1 is irrelevant. */
        return 0;

    ensure_n(n);
    return cache[n - 1];
}
