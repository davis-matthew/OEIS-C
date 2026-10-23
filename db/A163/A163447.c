/*
 * A163447  a(n) = 18*a(n-1) - 79*a(n-2) for n > 1; a(0) = 1, a(1) = 11.
 * https://oeis.org/A163447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 18*a(n-1) - 79*a(n-2), a(0)=1, a(1)=11
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cached sequence values */
static idx_t cache_len = 0;   /* number of elements currently cached */

/* Compute a(n) for any n >= 0 */
seq_t a(idx_t n)
{
    /* If we already have the value, return it. */
    if (n < cache_len)
        return cache[n];

    /* Ensure cache has at least the first two values. */
    if (cache_len == 0) {
        cache_len = 2;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_len);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)11;
    }

    /* Grow the cache to hold up to n. */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        cache = (seq_t*)realloc(cache, sizeof(seq_t) * new_len);
        /* Fill missing terms using the recurrence. */
        for (idx_t i = cache_len; i <= n; ++i)
            cache[i] = (seq_t)18 * cache[i - 1] - (seq_t)79 * cache[i - 2];
        cache_len = new_len;
    }

    return cache[n];
}
