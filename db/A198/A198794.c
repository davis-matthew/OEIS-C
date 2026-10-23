/*
 * A198794  a(n) = 5*6^n - 1.
 * https://oeis.org/A198794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*6^n - 1 Recurrence: a(0) = 4; a(n) = 6*a(n-1) + 5 for n
 *            > 0 The function uses memoisation so that calls may arrive in
 *            any order
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to store already computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;          /* number of stored entries */

    /* initialise cache on first call */
    if (!cache)
    {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failed – unlikely */
        cache[0] = (seq_t)4;           /* a(0) */
    }

    /* grow the cache if needed */
    if (n + 1 > cache_cap)
    {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed – unlikely */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values with the recurrence */
    for (size_t i = cache[0] ? 1 : cache_cap; i <= (size_t)n; ++i)
    {
        cache[i] = 6 * cache[i - 1] + 5;
    }

    return cache[n];
}
