/*
 * A153261  Primes p such that 3*p-2 is not prime.
 * https://oeis.org/A153261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n-th prime p such that 3*p-2 is NOT prime.
   The function incrementsally searches primes, tests 3*p-2 for
   compositeness, and memoises all found terms. */

/* Use the utilities supplied by the build */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>          /* for uint64_t if needed */
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t) */

/* ------------------------------------------------------------------ */
/* External types supplied by the build:                            */
/*   idx_t  – unsigned 64‑bit integer (index type)                   */
/*   seq_t  – integer type (8, 16, 32, 64‑bit or arbitrary precision) */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* Guard for non‑positive indices – the OEIS offset is 1. */
    if (n == 0) {
        return 0;
    }

    /* ------------------------------------------------------------------ */
    /* Memoised cache of terms already computed.                           */
    /* ------------------------------------------------------------------ */
    static seq_t *cache      = NULL;      /* array of valid primes   */
    static size_t cache_cnt  = 0;          /* number of stored terms */
    static size_t cache_cap  = 0;          /* capacity of the array  */

    /* ------------------------------------------------------------------ */
    /* Candidate prime generator                                         */
    /* ------------------------------------------------------------------ */
    static seq_t cand = 2;                 /* next number to test      */
    static bool first = true;              /* initialise after 2       */

    /* ------------------------------------------------------------------ */
    /* Ensure we have at least n terms in the cache.                     */
    /* ------------------------------------------------------------------ */
    while (cache_cnt < n)
    {
        seq_t p = cand;

        /* Advance candidate for next iteration   */
        if (first) {
            first = false;                  /* 2 has been processed   */
            cand = 3;                       /* next candidate 3       */
        } else {
            cand += 2;                      /* skip evens             */
        }

        /* Skip if p is not prime.               */
        if (!is_prime_big(p))
            continue;

        /* Compute 3*p-2 and test its primality. */
        seq_t t = 3 * p - 2;
        if (!is_prime_big(t))
        {
            /* Grow cache if needed.             */
            if (cache_cnt >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {                 /* allocation failed     */
                    exit(EXIT_FAILURE);
                }
                cache    = tmp;
                cache_cap = new_cap;
            }
            cache[cache_cnt++] = p;
        }
    }

    return cache[n - 1];
}
