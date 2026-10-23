/*
 * A142791  Primes congruent to 29 mod 60.
 * https://oeis.org/A142791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 411, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = nth prime congruent to 29 mod 60 (searched sequentially) */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"

/* Computes the n-th prime of the form 60k+29.  The function
   keeps an internal cache of already found terms and extends it
   as needed.  Candidate values are tested for primality using
   isPrimeGeneric().  */

/* Return the n-th term.  n is 1‑based. */
seq_t a(idx_t n)
{
    /* Static cache data (0‑based indices) */
    static seq_t *cache = NULL;    /* array of found terms                 */
    static idx_t cache_len = 0;    /* number of terms stored               */
    static idx_t cap = 0;          /* allocated capacity of cache          */
    static seq_t next_candidate = 29; /* next number to test (60k+29)        */

    /* Extend the cache until it contains the nth term. */
    while (cache_len < n) {
        seq_t cand = next_candidate;            /* current candidate */
        if (isPrimeGeneric(cand)) {              /* test for primality */
            /* Grow the cache if necessary */
            if (cache_len == cap) {
                cap = (cap == 0) ? 64 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                if (!tmp) {                     /* allocation failed */
                    return 0;                   /* graceful failure   */
                }
                cache = tmp;
            }
            cache[cache_len++] = cand;           /* store the prime   */
        }
        /* Advance to the next candidate of the form 60k+29. */
        next_candidate = cand + (seq_t)60;
    }

    /* Return the requested term (1‑based index). */
    return cache[n - 1];
}
