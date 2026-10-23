/*
 * A182229  a(n) = a(n-1) + floor(a(n-2)/3) with a(0)=2, a(1)=3.
 * https://oeis.org/A182229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + floor(a(n-2)/3), a(0)=2, a(1)=3
 * Range:     u8 n <= 21, u16 n <= 45, u32 n <= 92, u64 n <= 187,
 *            u128 n <= 377, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    if (n < cache_size)
        return cache[n];

    /* Allocate or grow the cache to hold indices up to n. */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) {
        /* Fallback to iterative computation without caching. */
        seq_t a0 = 2, a1 = 3, ai = 0;
        if (n == 0) return a0;
        if (n == 1) return a1;
        for (idx_t i = 2; i <= n; ++i) {
            ai = a1 + (a0 / 3);
            a0 = a1;
            a1 = ai;
        }
        return ai;
    }
    cache = tmp;

    /* Initialize the first terms if the cache was just created. */
    if (cache_size == 0 || cache_size == 1) {
        cache[0] = 2;
        if (new_size > 1)
            cache[1] = 3;
    }

    /* Compute new entries. */
    idx_t start = cache_size;
    if (start == 0)
        start = 2;           /* 0 and 1 are already initialized */
    else if (start == 1)
        start = 2;           /* 1 already initialized */

    for (idx_t i = start; i <= n; ++i)
        cache[i] = cache[i - 1] + (cache[i - 2] / 3);

    cache_size = new_size;
    return cache[n];
}
