/*
 * A126121  Numerators of sequence of fractions with e.g.f. sqrt(1+x)/(1-x)^2.
 * https://oeis.org/A126121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(0) = 1, a(1) = 5, a(n) = 5*a(n-1) + 2*(2*n-1)*(n-1)*a(n-2) for
 *            n >= 2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 27,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility to grow a dynamic array safely */
#include "../utils/ensure_capacity.h"
#include <stddef.h>

static seq_t *cache = NULL;      /* memoized values */
static size_t cache_len = 0;     /* number of valid entries in cache */
static size_t cache_cap = 0;     /* allocated capacity of cache */

/* Compute a(n) using the recurrence and memoize results */
seq_t a(idx_t n)
{
    /* Ensure cache has space for indices up to n */
    if ((size_t)n + 1 > cache_len)
    {
        size_t new_len = (size_t)n + 1;
        ensure_capacity(new_len, &cache_cap, (void **)&cache, sizeof(seq_t));

        /* If cache is growing from zero, set base values */
        if (cache_len == 0)
        {
            cache[0] = (seq_t)1;
            if (new_len > 1)
                cache[1] = (seq_t)5;
        }
        else if (cache_len == 1) /* when n==1 and cache_len==1 */
        {
            cache[1] = (seq_t)5;
        }

        /* Compute missing terms up to new_len-1 */
        for (size_t i = cache_len; i < new_len; ++i)
        {
            if (i == 0)
                cache[i] = (seq_t)1;
            else if (i == 1)
                cache[i] = (seq_t)5;
            else
            {
                /* a(i) = 5*a(i-1) + 2*(2*i-1)*(i-1)*a(i-2) */
                seq_t coeff = 2;                                       /* coeff = 2. */
                coeff *= (seq_t)(2 * (idx_t)i - 1);                    /* *(2*i-1) */
                coeff *= (seq_t)((idx_t)i - 1);                       /* * (i-1) */
                seq_t term1 = 5 * cache[i - 1];                        /* 5*a(i-1)  */
                seq_t term2 = coeff * cache[i - 2];                    /* coeff*a(i-2) */
                cache[i] = term1 + term2;
            }
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
