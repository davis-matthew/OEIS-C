/*
 * A023710  Numbers with a single 1 in their base 4 expansion.
 * https://oeis.org/A023710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth positive integer whose base‑4 representation contains
 *            exactly one digit 1
 * Range:     u8 n <= 108, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function builds the sequence on demand and memoises the results so that it can
   handle multiple calls in arbitrary order.                         */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache      = NULL;           /* array of computed terms          */
    static size_t  cache_cap= 0,  cache_len=0; /* current capacity and length      */
    static idx_t   current_candidate = 1;      /* next integer to test             */

    /* ensure we have at least n terms in the cache */
    while (cache_len < n)
    {
        /* grow the cache array as needed */
        if (cache_len == cache_cap)
        {
            size_t new_cap = (cache_cap == 0) ? 64 : cache_cap << 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) /* allocation failed – abort gracefully */
                return 0;
            cache      = tmp;
            cache_cap  = new_cap;
        }

        /* find the next integer with a single 1 in base‑4 */
        for (; ; ++current_candidate)
        {
            idx_t  x      = current_candidate;
            int    cnt1   = 0;

            /* count digits equal to 1 */
            while (x)
            {
                if ((x & 3ULL) == 1) /* x % 4 == 1  */
                {
                    ++cnt1;
                    if (cnt1 > 1) break;
                }
                x >>= 2; /* x /= 4  */
            }

            if (cnt1 == 1)
            {
                /* store the found value */
                cache[cache_len++] = (seq_t)current_candidate;
                ++current_candidate; /* next search starts after this number */
                break;
            }
        }
    }

    /* return the requested entry (indices start at 1) */
    return cache[n - 1];
}
