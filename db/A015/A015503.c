/*
 * A015503  a(1) = 1, a(n) = Sum_{k=1..n-1} ((4^k - 1)/3)*a(k).
 * https://oeis.org/A015503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1; a(2)=1; For n >= 3, a(n) = ((4^(n‑1)+2)/3) * a(n-1)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* malloc, realloc, abort */
#include <stdint.h>  /* for abort */

seq_t a(idx_t n)
{
    /* Memoisation cache: indices 1 .. last */
    static seq_t *cache   = NULL;
    static size_t   cap   = 0;
    static idx_t    last  = 0;
    static seq_t    pow4  = 0;   /* 4^(last-1) */

    /* Initialise on first call */
    if (!cache)
    {
        size_t init = 1;   /* enough for a(1) */
        cache = (seq_t *)malloc((init + 1) * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)0;          /* unused index 0 */
        cache[1] = (seq_t)1;          /* a(1) */
        cap   = init;
        last  = 1;
        pow4  = (seq_t)1;              /* 4^(1-1) = 1 */
    }

    /* Return cached value if already computed */
    if (n <= last)
        return cache[n];

    /* Grow cache if needed */
    if (n > cap)
    {
        size_t newcap = (cap ? cap * 2 : 1);
        while (newcap < (size_t)n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, (newcap + 1) * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap   = newcap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i)
    {
        /* Update pow4 to 4^(i-1) */
        pow4 *= (seq_t)4;

        if (i == 2)
        {
            cache[i] = (seq_t)1;          /* a(2) = 1 */
        }
        else
        {
            seq_t factor = pow4 + (seq_t)2;
            factor /= (seq_t)3;
            cache[i] = factor * cache[i - 1];
        }
    }

    last = n;
    return cache[n];
}
