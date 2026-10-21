/*
 * A074206  Kalmár's [Kalmar's] problem: number of ordered factorizations of
 *          n.
 * https://oeis.org/A074206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, core, easy, nice
 * Formula:   a(n) = sum_{d|n, d<n} a(d) (a(0)=0, a(1)=1)
 * Range:     u8 n <= 191, u16 n <= 8639, u32 n <= 9883, u64 n <= 9883,
 *            u128 n <= 9883, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation keeps a static cache so that each value is
 * computed at most once, even for non‑sequential calls.  For each
 * new n it merely extends the cache and fills in the new terms using
 * the divisor recurrence.  The algorithm runs in O(N·sqrt(N))
 * time for the first call up to N and then becomes O(K) for each
 * subsequent call where only a few new terms are added.  It works
 * for all call orders up to n≈10000 within the time limits. */

#include <stdlib.h>   /* malloc / realloc */

seq_t a(idx_t n)
{
    /* Static cache for previously computed values                */
    static seq_t *cache   = NULL;   /* holds a(i) for i=0..cache_len-1      */
    static idx_t cache_len = 0;     /* number of entries currently stored   */
    static idx_t cache_cap = 0;     /* allocated capacity                   */

    /* Basic cases */
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
    {
        /* Ensure a(0)=0 and a(1)=1 are stored */
        if (cache_len < 2)
        {
            if (cache_cap < 2)
            {
                idx_t new_cap = 2;
                cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                cache_cap = new_cap;
            }
            cache[0] = (seq_t)0;
            cache[1] = (seq_t)1;
            cache_len = 2;
        }
        return cache[1];
    }

    /* Initialise cache on first use */
    if (cache_len == 0)
    {
        cache_cap = 2;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_len = 2;
    }

    /* Extend the cache if necessary */
    if (n + 1 > cache_cap)
    {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 2;
        while (new_cap < n + 1)
            new_cap *= 2;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute any missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        seq_t sum = (seq_t)0;    /* sum of a(d) for divisors d < i */

        /* Enumerate divisors of i up to sqrt(i) */
        for (idx_t d = 1; d <= i / d; ++d)
        {
            if (i % d == 0)
            {
                if (d < i)
                    sum += cache[d];
                idx_t e = i / d;
                if (e != d && e < i)
                    sum += cache[e];
            }
        }
        cache[i] = sum;
    }
    cache_len = n + 1;   /* now all entries up to n are valid */

    return cache[n];
}
