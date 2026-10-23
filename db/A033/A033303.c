/*
 * A033303  Expansion of (1 + x)/(1 - 2*x - x^2 + x^3).
 * https://oeis.org/A033303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - a(n-3) a(0) = 1, a(1) = 3, a(2) = 7
 *            Recurrence valid for n >= 3
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 54, u128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cache of computed terms */
static size_t cache_size = 0;        /* number of terms currently stored (cache[0..cache_size-1]) */
static size_t cache_cap = 0;          /* allocated capacity of cache */

seq_t a(idx_t n)
{
    /* handle n less than 3 after cache initialisation */
    size_t idx = (size_t)n;          /* use size_t for array indices */

    /* initialise cache on first call */
    if (cache == NULL)
    {
        size_t init_cap = 4;           /* start with a small but non-zero capacity */
        if (init_cap < 3) init_cap = 3;
        if (init_cap < idx + 1) init_cap = idx + 1;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        cache_cap = init_cap;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)7;
        cache_size = 3;
    }

    if (idx < cache_size)            /* already known */
        return cache[idx];

    /* grow cache to accommodate up to n */
    while (cache_cap <= idx)
    {
        size_t new_cap = (cache_cap ? cache_cap * 2 : 4);
        if (new_cap < idx + 1) new_cap = idx + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute missing terms iteratively using recurrence */
    for (size_t i = cache_size; i <= idx; ++i)
    {
        seq_t val = (seq_t)2 * cache[i - 1] + cache[i - 2] - cache[i - 3];
        cache[i] = val;
    }
    cache_size = idx + 1;
    return cache[idx];
}
