/*
 * A167442  Prime numbers ending in the prime number 11.
 * https://oeis.org/A167442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 165, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Procedure: a(n) = the n-th prime of the form 100*k + 11 (k ≥ 0). */
/* A sequential search is used; results are cached to allow arbitrary
   order of calls. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined, but harmless */

    /* Cache of found primes: cache[i] holds the (i+1)‑th prime of the form. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated length of cache */
    static size_t cache_len = 0;   /* number of primes actually stored */
    static idx_t next_i = 0;       /* index of the next candidate 100*k+11 */

    while (cache_len < (size_t)n)
    {
        /* Candidate prime 11 + 100*next_i. */
        seq_t cand = 11 + (seq_t)(next_i * 100ULL);

        if (isPrimeGeneric(cand))
        {
            /* Grow the cache if required. */
            if (cache_len + 1 > cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                void *p = realloc(cache, new_cap * sizeof(seq_t));
                if (!p) abort();
                cache = (seq_t *)p;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
        ++next_i;
    }

    return cache[n - 1];
}
