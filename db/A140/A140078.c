/*
 * A140078  Numbers k such that k and k+1 have 4 distinct prime factors.
 * https://oeis.org/A140078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 348, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A140078: the numbers k such that k and k+1 have
   exactly 4 distinct prime factors.
   The function searches integers incrementally, memoising found terms. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/count_distinct_primes.h"

/* External linkage */
seq_t a(idx_t n)
{
    if (n == 0) {    /* undefined for offset 1 – return 0 as a safety value */
        return (seq_t)0;
    }

    /* --- static memoisation state --------------------------------------- */
    static seq_t *cache      = NULL;   /* array holding found terms    */
    static idx_t  cache_len  = 0;      /* number of terms stored       */
    static idx_t  cache_cap  = 0;      /* current allocated capacity   */
    static uint64_t next_candidate = 2;/* next integer to test         */

    /* Extend cache until at least n terms are stored. */
    while (cache_len < n)
    {
        /* Find the next k satisfying the property. */
        for (;;)
        {
            uint64_t k = next_candidate;
            if (count_distinct_primes(k)     == 4 &&
                count_distinct_primes(k + 1) == 4)
            {
                /* Store value. The cast stores the truncated value if
                   seq_t is smaller than 64 bits. */
                seq_t val = (seq_t)k;

                /* Grow cache if necessary. */
                if (cache_len == cache_cap)
                {
                    idx_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) abort();          /* Out of memory */
                    cache      = tmp;
                    cache_cap  = new_cap;
                }

                /* Append value. */
                cache[cache_len++] = val;
                ++next_candidate;              /* Advance for next search */
                break;
            }
            ++next_candidate;
        }
    }

    /* Return the (n‑1)th element (zero‑based). */
    return cache[n - 1];
}
