/*
 * A193007  Coefficient of x in the reduction by x^2->x+1 of the polynomial
 *          p(n,x) defined at Comments.
 * https://oeis.org/A193007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 9*a(n-2) + 6*a(n-3) + a(n-4) - 3*a(n-5) +
 *            a(n-6) with a(0)=0, a(1)=1, a(2)=1, a(3)=9, a(4)=36, a(5)=108
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#if defined(__SIZEOF_INT128__)
typedef unsigned __int128 u128;
#endif

seq_t a(idx_t n)
{
    /* Local static cache to avoid recomputation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* highest index computed so far (valid when cache != NULL) */
    static size_t cache_cap = 0;

    /* Initial terms of the sequence */
    static const seq_t init[6] = {0, 1, 1, 9, 36, 108};

    /* First call – allocate the cache and store the initial terms */
    if (cache == NULL)
    {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)
            return 0; /* Allocation failure → fallback */

        for (idx_t i = 0; i < 6 && i < cache_cap; ++i)
            cache[i] = init[i];

        cache_len = 5; /* highest index stored */
    }

    /* Grow the buffer if the requested index is outside the current capacity */
    if ((size_t)n + 1 > cache_cap)
    {
        size_t new_cap = cache_cap;
        while ((size_t)n + 1 > new_cap)
            new_cap <<= 1;           /* double the size each step */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            return 0; /* Realloc failure → fallback */

        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms up to n, if needed */
    for (idx_t i = cache_len + 1; i <= n; ++i)
    {
        if (i < 6)          /* safety guard – initial terms were already set */
            cache[i] = init[i];
        else
        {
#if defined(__SIZEOF_INT128__)
            u128 t1 = (u128)cache[i - 1] * 5
                    + (u128)cache[i - 3] * 6
                    + (u128)cache[i - 4]
                    + (u128)cache[i - 6];
            u128 t2 = (u128)cache[i - 2] * 9
                    + (u128)cache[i - 5] * 3;
            cache[i] = (seq_t)(t1 - t2);
#else
            seq_t t1 = cache[i - 1] * 5
                     + cache[i - 3] * 6
                     + cache[i - 4]
                     + cache[i - 6];
            seq_t t2 = cache[i - 2] * 9
                     + cache[i - 5] * 3;
            cache[i] = t1 - t2;
#endif
        }
    }

    cache_len = n;
    return cache[n];
}
