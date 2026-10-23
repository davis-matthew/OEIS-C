/*
 * A135246  Shifted Pell recurrence: a(n) = 2*a(n-2) + a(n-4).
 * https://oeis.org/A135246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-2) + a(n-4); a(0)=1, a(1)=3, a(2)=5, a(3)=7
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 48, u64 n <= 99, u128 n <= 199,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t max_computed = 0;
static int initialized = 0;

seq_t a(idx_t n)
{
    /* Ensure storage for indices up to n */
    size_t need = (size_t)n + 1;
    if (need > cache_cap)
    {
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = need;
    }

    /* Initialize first four terms, if not already done */
    if (!initialized)
    {
        if (cache_cap < 4)
        {
            seq_t *tmp = (seq_t *)realloc(cache, 4 * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = 4;
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)7;
        max_computed = 3;
        initialized = 1;
    }

    /* Compute any missing terms up to n */
    if (n > max_computed)
    {
        size_t start = (size_t)max_computed + 1;
        if (start < 4) start = 4;
        for (size_t i = start; i <= (size_t)n; ++i)
        {
            cache[i] = (seq_t)2 * cache[i - 2] + cache[i - 4];
        }
        max_computed = n;
    }

    return cache[(size_t)n];
}
