/*
 * A041025  Denominators of continued fraction convergents to sqrt(17).
 * https://oeis.org/A041025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0) = 1, a(1) = 8, a(n) = 8*a(n-1) + a(n-2) (n >= 2)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
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
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;

    /* allocate initial cache if needed */
    if (cache == NULL)
    {
        /* start with two slots for a(0) and a(1) */
        size_t init_cap = 2;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        cache_cap = init_cap;
        cache_len = 0;
    }

    /* grow cache if requested n is outside current capacity */
    if (n >= cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 2;
        while (n >= new_cap)
            new_cap *= 2;

        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        /* copy existing terms */
        for (idx_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];

        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    if (cache_len == 0)
    {
        cache[0] = 1;
        cache_len = 1;
    }
    if (cache_len == 1)
    {
        cache[1] = 8;
        cache_len = 2;
    }

    for (idx_t i = cache_len; i <= n; ++i)
    {
        /* a(i) = 8*a(i-1) + a(i-2) */
        cache[i] = 8 * cache[i - 1] + cache[i - 2];
    }
    cache_len = n + 1;

    return cache[n];
}
