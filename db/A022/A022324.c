/*
 * A022324  a(n) = a(n-1) + a(n-2) + 1, with a(0) = 1 and a(1) = 10.
 * https://oeis.org/A022324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + 1 (a(0)=1, a(1)=10)
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;

    /* Initialise base values on first call. */
    if (cache_cap == 0) {
        cache_cap = 2;
        cache = (seq_t *)malloc((size_t)cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)10;
    }

    if (n < cache_cap)
        return cache[n];

    /* Grow cache to size n+1. */
    idx_t old_cap = cache_cap;
    idx_t new_cap = n + 1;
    if (new_cap > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (size_t)new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute the missing terms. */
    for (idx_t i = old_cap; i <= n; ++i) {
        cache[i] = cache[i - 1] + cache[i - 2] + (seq_t)1;
    }

    return cache[n];
}
