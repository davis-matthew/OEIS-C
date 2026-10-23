/*
 * A029373  Expansion of 1/((1-x^4)*(1-x^8)*(1-x^11)*(1-x^12)).
 * https://oeis.org/A029373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 4, 8, 11, and 12
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* DP implementation of the coin‑change recurrence. */
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_limit = 0; /* number of valid indices in cache */

seq_t a(idx_t n) {
    /* grow cache and recompute values up to n if necessary */
    if (n + 1 > cache_limit) {
        idx_t new_limit = n + 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_limit);
        for (idx_t i = 0; i < new_limit; ++i)
            cache[i] = 0;
        cache[0] = 1;                       /* base case */
        const idx_t parts[4] = {4, 8, 11, 12};
        for (int p_i = 0; p_i < 4; ++p_i) {
            idx_t p = parts[p_i];
            for (idx_t i = p; i < new_limit; ++i)
                cache[i] += cache[i - p];
        }
        cache_limit = new_limit;
    }
    return cache[n];
}
