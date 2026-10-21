/*
 * A229731  G.f.: x^4*(6 + x - 7*x^2 - x^3 + 3*x^4 + x^5)/(1-x-x^2)^3.
 * https://oeis.org/A229731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. = x^4*(6 + x - 7*x^2 - x^3 + 3*x^4 + x^5)/(1-x-x^2)^3
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 35, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* currently allocated size */
    static idx_t cache_size = 0;  /* number of filled entries */

    /* Base values for n = 0 … 9 from the sequence definition */
    static const seq_t base[10] =
    {
        0, 0, 0, 0, 6, 19, 50, 119, 265, 564
    };

    /* Ensure the cache can hold indexes up to n */
    if (n + 1 > cache_cap)
    {
        idx_t new_cap = cache_cap > 0 ? cache_cap * 2 : 16;
        if (new_cap < n + 1)
            new_cap = n + 1;

        seq_t *new_cache;
#if defined(__cplusplus)
        new_cache = new seq_t[new_cap];
        for (idx_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
#else
        new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* realloc preserves the existing contents */
#endif
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n using the recurrence:
       a(n) = 3*a(n-1) - 5*a(n-3) + 3*a(n-5) + a(n-6)  for n >= 10   */
    if (n + 1 > cache_size)
    {
        for (idx_t i = cache_size; i <= n; ++i)
        {
            if (i < 10)
            {
                cache[i] = base[i];
            }
            else
            {
                cache[i] =
                    (seq_t)3 * cache[i - 1]
                    - (seq_t)5 * cache[i - 3]
                    + (seq_t)3 * cache[i - 5]
                    +        cache[i - 6];
            }
        }
        cache_size = n + 1;
    }

    return cache[n];
}
