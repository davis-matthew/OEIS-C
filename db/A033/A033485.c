/*
 * A033485  a(n) = a(n-1) + a(floor(n/2)), a(1) = 1.
 * https://oeis.org/A033485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(1) = 1, a(n) = a(n-1) + a(floor(n/2)) for n ≥ 2
 * Range:     u8 n <= 21, u16 n <= 90, u32 n <= 620, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache – grows to whatever n is requested */
    static seq_t *cache = NULL;   /* array 1‑based, zero element unused */
    static idx_t cap   = 0;       /* allocated capacity (size of array)   */
    static idx_t last  = 0;       /* largest n for which cache[n] is valid */

    if (n == 0)                /* offset is 1 – undefined for 0 */
        return (seq_t)0;

    /* grow the cache if needed */
    if (!cache || n >= cap)
    {
        size_t newcap = (size_t)n + 1;      /* need indices 0 … n        */
        seq_t *p = NULL;

        if (cache)                         /* existing array – realloc */
            p = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        else                               /* first allocation – calloc */
            p = (seq_t *)calloc(newcap, sizeof(seq_t));

        if (!p)                            /* allocation failed      */
            return (seq_t)0;

        cache = p;
        cap   = newcap;
    }

    /* initialise the first term if this is the first call */
    if (last == 0)
    {
        cache[1] = (seq_t)1;
        last    = 1;
    }

    /* compute missing terms up to n (one forward pass) */
    if (n > last)
    {
        for (idx_t i = last + 1; i <= n; ++i)
            cache[i] = cache[i - 1] + cache[i / 2];
        last = n;
    }

    return cache[n];
}
