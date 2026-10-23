/*
 * A132868  a(n) = 3*a(n-1) - a(n-3) + 3*a(n-4), with initial values 1,3,7,20.
 * https://oeis.org/A132868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-3) + 3*a(n-4); Initial values: a(0)=1,
 *            a(1)=3, a(2)=7, a(3)=20
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;

    /* initial terms */
    static const seq_t init_vals[4] = {1, 3, 7, 20};

    /* small n handled directly */
    if (n < 4)
        return init_vals[(size_t)n];

    /* initialise cache on first call or grow if needed */
    if (cache_len == 0)
    {
        /* ensure capacity for at least n+1 entries */
        cache_len = (size_t)n + 1;
        if (cache_len < 4)
            cache_len = 4;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache)
            abort();          /* out of memory */

        /* store initial values */
        cache[0] = init_vals[0];
        cache[1] = init_vals[1];
        cache[2] = init_vals[2];
        cache[3] = init_vals[3];

        /* compute up to n */
        for (size_t i = 4; i <= (size_t)n; ++i)
            cache[i] = 3 * cache[i - 1] - cache[i - 3] + 3 * cache[i - 4];

        return cache[(size_t)n];
    }

    /* grow cache if requested n exceeds current size */
    if (cache_len <= (size_t)n)
    {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)
            abort();  /* out of memory */
        cache = tmp;

        /* compute new entries */
        for (size_t i = cache_len; i <= (size_t)n; ++i)
            cache[i] = 3 * cache[i - 1] - cache[i - 3] + 3 * cache[i - 4];

        cache_len = new_len;
    }

    return cache[(size_t)n];
}
