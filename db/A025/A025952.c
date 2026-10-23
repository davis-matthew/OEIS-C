/*
 * A025952  Expansion of 1/((1-2*x)*(1-3*x)*(1-9*x)*(1-10*x)).
 * https://oeis.org/A025952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=24
   a(n) = 3^(n+1) - 2^(n+1) + 19*a(n-1) - 90*a(n-2)  (n >= 2)       */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache = NULL;
    static idx_t  last_n = 0;          /* highest index computed */
    static seq_t  pw2 = 0, pw3 = 0;    /* 2^(last_n+1) and 3^(last_n+1)   */
    static size_t cache_size = 0;      /* size of allocated cache      */

    /* initialise cache for n = 0 and n = 1 */
    if (cache == NULL)
    {
        cache_size = 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)24;
        last_n   = 1;
        /* pw2 and pw3 correspond to powers needed for n = 2: 2^2 and 3^2   */
        pw2 = (seq_t)4;     /* 2^(last_n+1) = 2^2   */
        pw3 = (seq_t)9;     /* 3^(last_n+1) = 3^2   */
        return cache[n];
    }

    /* if requested index already computed, return it */
    if (n <= last_n)
        return cache[(size_t)n];

    /* extend cache if necessary */
    if (n + 1 > cache_size)
    {
        size_t new_size = (size_t)(n) + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) /* In production, handle allocation failure */
            return 0;
        cache     = new_cache;
        cache_size = new_size;
    }

    /* compute missing terms up to n */
    for (idx_t i = last_n + 1; i <= n; ++i)
    {
        /* Update the small powers */
        pw2 = pw2 * (seq_t)2;   /* 2^(i+1)            */
        pw3 = pw3 * (seq_t)3;   /* 3^(i+1)            */

        /* recurrence: a(i) = pw3 - pw2 + 19*a(i-1) - 90*a(i-2) */
        seq_t term = pw3 - pw2
                   + (seq_t)19 * cache[(size_t)(i - 1)]
                   - (seq_t)90 * cache[(size_t)(i - 2)];

        cache[(size_t)i] = term;
    }

    last_n = n;
    return cache[(size_t)n];
}
