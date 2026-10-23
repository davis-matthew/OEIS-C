/*
 * A159328  Transform of 1 by the T_{1,1} transformation (see link).
 * https://oeis.org/A159328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n+3)=3*a(n+2)-2*a(n+1)+a(n), with a(0)=2, a(1)=4, a(2)=10
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 25, u64 n <= 51, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Grow the cache to at least the index n (inclusive). */
static void ensure_cache_size(seq_t **cache, size_t *cap, idx_t n)
{
    size_t required = (size_t)n + 1;
    if (required <= *cap) return;

    size_t new_cap = (*cap == 0) ? 4 : *cap;
    while (new_cap < required) new_cap <<= 1;

    *cache = (*cache == NULL)
                ? (seq_t *)malloc(new_cap * sizeof(seq_t))
                : (seq_t *)realloc(*cache, new_cap * sizeof(seq_t));

    *cap = new_cap;
}

/* OEIS A159328: recursive definition with memoisation. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* memoisation array */
    static size_t cache_cap = 0;     /* allocated size      */
    static idx_t  cache_len = 0;     /* number of values stored */

    /* Ensure the cache can hold up to index n. */
    ensure_cache_size(&cache, &cache_cap, n);

    /* Compute missing values up to n. */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        if (i == 0)
            cache[0] = (seq_t)2;
        else if (i == 1)
            cache[1] = (seq_t)4;
        else if (i == 2)
            cache[2] = (seq_t)10;
        else
            cache[i] = 3 * cache[i-1] - 2 * cache[i-2] + cache[i-3];
    }

    /* Update the stored length. */
    if (cache_len <= n) cache_len = n + 1;

    return cache[n];
}
