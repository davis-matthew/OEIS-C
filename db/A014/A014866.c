/*
 * A014866  Numbers k that divide s(k), where s(1)=1, s(j)=16*s(j-1)+j.
 * https://oeis.org/A014866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k such that k divides the integer s(k),
 *            where s(1)=1 and s(j)=16*s(j-1)+j for j>1. The function computes
 *            terms on demand, caching all previously found values. It uses
 *            only integer arithmetic, supports any unsigned seq_t (8, 16, 32,
 *            64 or 128 bits, or an arbitrary‑precision...
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* the function must be defined with external linkage */
seq_t a(idx_t n)
{
    /* Offset is 1; 0 is never queried, but guard against it. */
    if (n == 0) return (seq_t)0;

    /* Static cache of already found terms */
    static seq_t *cache      = NULL;   /* array of terms */
    static size_t cache_cap  = 0;      /* allocated capacity */
    static size_t cache_size = 0;      /* current number of terms stored */
    static idx_t  last_k_checked = 0;  /* largest k processed so far */

    /* Grow the cache until we have at least n terms. */
    while (cache_size < n)
    {
        idx_t k = last_k_checked + 1;
        unsigned __int128 r = 1;                 /* s(1) mod k  (k>=1) */
        if (k > 1)
        {
            r %= k;
            for (idx_t j = 2; j <= k; ++j)
            {
                /* Compute (16*r + j) % k safely using 128‑bit intermediate. */
                unsigned __int128 t = (__uint128_t)16 * r + j;
                r = t % k;
            }
        }
        else
        {
            r %= k;   /* r == 0 when k==1 */
        }

        if ((unsigned long long)r == 0)
        {
            /* Allocate or grow storage for the cache. */
            if (cache_size >= cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache,
                                               new_cap * sizeof(seq_t));
                if (tmp == NULL) abort();     /* standard error handling */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)k;
        }

        last_k_checked = k;
    }

    return cache[n - 1];
}
