/*
 * A041262  Numerators of continued fraction convergents to sqrt(143).
 * https://oeis.org/A041262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A041262 - Numerators of continued fraction convergents to sqrt(143)
 * Recurrence: a(0) = 11, a(1) = 12, a(2) = 275, a(3) = 287,
 *              a(n) = 24*a(n-2) - a(n-4)  for n ≥ 4
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache of computed terms a(0)..a(k) */
    static idx_t cache_len = 0;   /* number of stored terms */

    /* if needed, grow the cache to contain a(n) */
    if (n + 1 > cache_len)
    {
        size_t new_len = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;   /* allocation failed */

        cache = new_cache;

        /* compute the terms that are now required */
        for (idx_t i = cache_len; i <= n; ++i)
        {
            if (i == 0)        cache[0] = (seq_t)11;
            else if (i == 1)   cache[1] = (seq_t)12;
            else if (i == 2)   cache[2] = (seq_t)275;
            else if (i == 3)   cache[3] = (seq_t)287;
            else               cache[i] = (seq_t)24 * cache[i - 2] - cache[i - 4];
        }
        cache_len = (idx_t)new_len;
    }

    return cache[n];
}
