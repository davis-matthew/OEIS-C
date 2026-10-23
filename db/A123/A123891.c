/*
 * A123891  Expansion of (1-3*x^2+x^3)/(1-3*x+x^3).
 * https://oeis.org/A123891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=3, a(2)=6, a(3)=18; for n>=4, a(n)=3*a(n-1)-a(n-3)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    /* ensure cache is large enough */
    if (cache == NULL)
    {
        cache_cap = 4;              /* need at least space for indices 0..3 */
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;        /* allocation failed; fallback */
    }
    else if (n + 1 > cache_cap)
    {
        idx_t new_cap = cache_cap * 2;
        if (new_cap < n + 1)
            new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;        /* allocation failed; fallback */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute any missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        if (i == 0)
            cache[0] = (seq_t)1;
        else if (i == 1)
            cache[1] = (seq_t)3;
        else if (i == 2)
            cache[2] = (seq_t)6;
        else if (i == 3)
            cache[3] = (seq_t)3 * cache[2] - cache[0] + (seq_t)1; /* 18 */
        else
            cache[i] = (seq_t)3 * cache[i - 1] - cache[i - 3];
    }
    cache_len = n + 1;

    return cache[n];
}
