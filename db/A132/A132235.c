/*
 * A132235  Primes congruent to 23 (mod 30).
 * https://oeis.org/A132235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 825, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of primes congruent to 23 (mod 30) with offset 1.
   (a(1)=23, a(2)=53, …) */

/*  The function uses a small expanding cache so that calls with any
    order of n are fast and do not recompute primes that were found
    previously.  It relies on the helper functions provided by the
    build environment (next_prime, seq_realloc, etc.). */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for offset 1, but harmless */

    /* Static cache of previously found terms */
    static seq_t *cache   = NULL;         /* pointer to array of seq_t */
    static size_t cache_len = 0;          /* number of elements stored */
    static size_t cache_cap = 0;          /* allocated capacity */

    /* The last number that was examined for primality.  We use a
       64‑bit unsigned integer because next_prime works on uint64_t. */
    static uint64_t last_checked = 1;     /* 1 keeps the first candidate as 2 */

    /* Ensure the cache is big enough to return the required value. */
    while (cache_len < (size_t)n)
    {
        /* Get the next prime after the previous one that was checked. */
        uint64_t cand = next_prime(last_checked + 1);
        last_checked = cand;

        /* Only remember it if it satisfies the congruence condition. */
        if ((cand % 30u) == 23u)
        {
            /* Resize the cache if necessary. */
            if (cache_len == cache_cap)
            {
                size_t new_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp)      /* allocation failed – return 0 as sentinel */
                    return (seq_t)0;
                cache       = tmp;
                cache_cap   = new_cap;
            }
            /* Store the prime in the cache. */
            cache[cache_len++] = (seq_t)cand;
        }
    }

    /* Return the requested term (1‑based index). */
    return cache[n - 1];
}
