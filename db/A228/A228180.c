/*
 * A228180  The number of single edges on the boundary of ordered trees with n
 *          edges.
 * https://oeis.org/A228180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((13*n-5)*a(n-1) - (23*n-37)*a(n-2) + (12*n-30)*a(n-3)) /
 *            (2*(n+1)) * with a(0)=0, a(1)=1, a(2)=2, a(3)=6. * The
 *            implementation uses a memoised cache; recursion is evaluated
 *            with * appropriate arithmetic to avoid overflow on 64‑bit builds
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 35, u128 n <= 67,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t sz = 0;

    /* extend cache up to n */
    if (n >= sz)
    {
        idx_t new_sz = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_sz * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;  /* allocation failed */

        /* copy existing values */
        for (idx_t i = 0; i < sz; ++i)
            new_cache[i] = cache[i];

        /* compute missing entries */
        for (idx_t i = sz; i <= n; ++i)
        {
            if (i == 0)                     new_cache[i] = 0;
            else if (i == 1)                new_cache[i] = 1;
            else if (i == 2)                new_cache[i] = 2;
            else if (i == 3)                new_cache[i] = 6;
            else
            {
                /* use 128‑bit intermediates for builtin types (≤ 8 bytes) */
                if (sizeof(seq_t) <= 8) /* e.g., uint64_t or smaller */
                {
                    unsigned __int128 m1 = (unsigned __int128)(13 * i - 5);
                    unsigned __int128 m2 = (unsigned __int128)(23 * i - 37);
                    unsigned __int128 m3 = (unsigned __int128)(12 * i - 30);
                    unsigned __int128 t1 = (unsigned __int128)new_cache[i-1] * m1;
                    unsigned __int128 t2 = (unsigned __int128)new_cache[i-2] * m2;
                    unsigned __int128 t3 = (unsigned __int128)new_cache[i-3] * m3;
                    unsigned __int128 num = t1 + t3 - t2;
                    unsigned __int128 denom = (unsigned __int128)2 * (i + 1);
                    new_cache[i] = (seq_t)(num / denom);
                }
                else /* arbitrary‑precision sequence type */
                {
                    seq_t t1 = mul_small(new_cache[i-1], (unsigned int)(13 * i - 5));
                    seq_t t2 = mul_small(new_cache[i-2], (unsigned int)(23 * i - 37));
                    seq_t t3 = mul_small(new_cache[i-3], (unsigned int)(12 * i - 30));
                    seq_t num = t1;
                    num += t3;
                    num -= t2;
                    seq_t denom = (seq_t)2 * ((seq_t)i + 1);
                    new_cache[i] = num / denom;
                }
            }
        }

        /* free old cache */
        free(cache);
        cache = new_cache;
        sz = new_sz;
    }

    return cache[n];
}
