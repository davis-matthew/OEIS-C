/*
 * A029073  Expansion of 1/((1-x)*(1-x^4)*(1-x^7)*(1-x^8)).
 * https://oeis.org/A029073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 1, 4, 7 and 8.
 *            Computed by dynamic programming: for each part p ∈ {1,4,7,8},
 *            for i = p … n: a[i] += a[i−p] (with a[0] = 1)
 * Range:     u8 n <= 59, u16 n <= 434, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* partition counts up to current maximum n */
static idx_t cache_size = 0;  /* number of entries currently cached */

/* Return the number of partitions of n into parts 1,4,7,8. */
seq_t a(idx_t n)
{
    /* Grow the cache to hold n. */
    if (n + 1 > cache_size) {
        /* Allocate or reallocate space for n+1 entries. */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!tmp) {
            /* allocation failed – return 0 as a safe fallback */
            return 0;
        }
        cache = tmp;
        cache_size = n + 1;
    }

    /* Recompute the entire DP table up to n (O(4·n) operations). */
    cache[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        cache[i] = 0;

    const idx_t parts[] = {1, 4, 7, 8};
    for (int p = 0; p < 4; ++p) {
        idx_t part = parts[p];
        for (idx_t i = part; i <= n; ++i) {
            cache[i] += cache[i - part];
        }
    }

    return cache[n];
}
