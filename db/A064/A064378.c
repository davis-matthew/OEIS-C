/*
 * A064378  a(0) = 2, a(n) = 2^(n+1)*(n-1)! (n >= 1).
 * https://oeis.org/A064378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 2, a(n) = 2^(n+1)·(n-1)! for n ≥ 1 Recurrence: a(n) =
 *            a(n-1) · 2 · (n-1) for n ≥ 2 (a(1) = 4 is handled separately)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;     /* number of entries stored (== cache_len-1 is highest index) */
    static size_t cache_cap = 0;     /* allocated capacity */

    /* lazy initialization of cache */
    if (cache == NULL)
    {
        cache_cap = 16;                            /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)                         /* allocation failure is not expected in tests */
            return (seq_t)0;
        cache[0] = (seq_t)2;                       /* a(0) */
        cache[1] = (seq_t)4;                       /* a(1) */
        cache_len = 2;                             /* we have values for indices 0 and 1 */
    }

    /* If the requested index is already cached, return it */
    if (n < cache_len)
        return cache[n];

    /* Ensure capacity to store values up to n */
    while (n >= cache_cap)
    {
        size_t new_cap = cache_cap * 2;
        if (new_cap == 0) new_cap = 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;          /* allocation failure – unlikely in tests */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute new values from the last cached index up to n */
    seq_t val = cache[cache_len - 1];   /* last computed value */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        val = val * (seq_t)(i - 1) * (seq_t)2;
        cache[i] = val;
    }
    cache_len = n + 1;
    return cache[n];
}
