/*
 * A168607  a(n) = 3^n + 2.
 * https://oeis.org/A168607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n + 2 Recurrence: a(0) = 3; a(n) = 3 * a(n-1) - 4 for n
 *            > 0 This implementation uses memoisation to be fast for any
 *            order of calls
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ensure_capacity.h"

static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    /* ensure the cache can hold up to index n */
    if (n >= cache_cap) {
        /* double current capacity or at least need n+1 elements */
        size_t needed = (size_t)n + 1;
        ensure_capacity(needed, &cache_cap, (void **)&cache, sizeof(seq_t));
    }

    /* initialise first element if cache is empty */
    if (cache_len == 0) {
        cache[0] = (seq_t)3;
        cache_len = 1;
    }

    /* compute missing terms up to n using the recurrence */
    while (cache_len <= n) {
        idx_t i = cache_len;                /* next index to compute */
        if (i == 0) {
            /* already initialised */
        } else {
            cache[i] = cache[i - 1] * (seq_t)3 - (seq_t)4;
        }
        cache_len = i + 1;
    }

    return cache[n];
}
