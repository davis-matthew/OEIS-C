/*
 * A035325  Primes of the form 2^k + k^2 + 1.
 * https://oeis.org/A035325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A035325: primes of the form 2^k + k^2 + 1.
   k is iterated over the multiples of 6 (A100357).  The function
   memoises results so repeated calls are fast.  Uses the helper
   is_prime_big() for primality testing. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Memoisation: cache of found terms. */
    static seq_t *cache = NULL;    /* array of primes found */
    static size_t cap = 0;          /* capacity of cache */
    static size_t sz  = 0;          /* number of filled entries */
    static idx_t  last_k = 0;       /* last k processed (multiple of 6) */
    static seq_t power2 = 1;        /* 2^last_k */

    /* Initialise cache on first call. */
    if (!cache) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        last_k = 0;        /* start with k = 0 */
        power2 = 1;        /* 2^0            */
    }

    /* If the desired term is already computed, return it. */
    if (sz >= n) return cache[n - 1];

    /* Generate primes until we have at least n terms. */
    while (sz < n) {
        /* Current k as seq_t for arithmetic. */
        seq_t k   = (seq_t)last_k;
        seq_t val = power2;       /* 2^k          */
        val += k * k;             /* + k^2        */
        val += 1;                 /* + 1          */

        /* Test primality. */
        if (is_prime_big(val)) {
            /* Grow cache if necessary. */
            if (sz >= cap) {
                cap *= 2;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[sz++] = val;
        }

        /* Advance to the next multiple of 6. */
        last_k += 6;
        /* Multiply power2 by 64 (2^6) for the new k. */
        power2 *= (seq_t)64;
    }

    return cache[n - 1];
}
