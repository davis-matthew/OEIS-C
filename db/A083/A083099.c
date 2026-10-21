/*
 * A083099  a(n) = 2*a(n-1) + 6*a(n-2), a(0) = 0, a(1) = 1.
 * https://oeis.org/A083099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 6*a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cache of computed terms */
static idx_t cache_len = 0;          /* number of entries currently stored */

static void ensure_cache(idx_t n)
{
    /* Expand the cache so that it holds indices up to n */
    if (n < cache_len)
        return;                      /* already sufficient */

    idx_t new_len = n + 1;            /* need indices 0..n inclusive */
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    cache = tmp;
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    /* Make sure the cache can hold the requested index */
    ensure_cache(n);

    /* Compute missing values in order, using the recurrence */
    for (idx_t i = 0; i <= n; ++i)
    {
        if (i == 0)
            cache[i] = 0;
        else if (i == 1)
            cache[i] = 1;
        else
            cache[i] = 2 * cache[i - 1] + 6 * cache[i - 2];
    }

    return cache[n];
}
