/*
 * A119886  a(n) = 20*a(n-2) - 64*a(n-4).
 * https://oeis.org/A119886
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*a(n-2) - 64*a(n-4) Initial values: a(0)=1, a(1)=59,
 *            a(2)=2416, a(3)=6230, a(4)=47680
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 12, u64 n <= 28, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap   = 0;

    /* Initialise the cache once with the first five terms. */
    if (cache_size == 0)
    {
        cache_cap   = 5;
        cache       = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)59;
        cache[2] = (seq_t)2416;
        cache[3] = (seq_t)6230;
        cache[4] = (seq_t)47680;
        cache_size = 5;
    }

    /* Grow the array if necessary. */
    if (n >= cache_cap)
    {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap <= n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Return cached value if already computed. */
    if (n < cache_size)
        return cache[n];

    /* Compute missing terms up to n. */
    for (idx_t i = cache_size; i <= n; ++i)
    {
        if (i < 5)            /* base terms */
        {
            /* cache is already initialised for i == 0..4 */
        }
        else
        {
            cache[i] = (seq_t)20 * cache[i - 2] - (seq_t)64 * cache[i - 4];
        }
    }

    cache_size = n + 1;
    return cache[n];
}
