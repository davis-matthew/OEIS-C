/*
 * A214887  a(n) = a(n-1)*a(n-2) with a(0)=1, a(1)=7.
 * https://oeis.org/A214887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) * a(n-2); a(0) = 1, a(1) = 7
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array of computed terms */
static size_t cache_len = 0;     /* number of elements stored */
static size_t cache_cap = 0;     /* allocated capacity of cache */

/* Ensure that cache is computed up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    /* Already computed? */
    if ((idx_t)cache_len > n)
        return;

    /* Grow the array if necessary */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < needed)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms */
    for (size_t i = cache_len; i < needed; ++i) {
        if (i == 0)
            cache[i] = (seq_t)1;
        else if (i == 1)
            cache[i] = (seq_t)7;
        else
            cache[i] = cache[i - 1] * cache[i - 2];
    }
    cache_len = needed;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
