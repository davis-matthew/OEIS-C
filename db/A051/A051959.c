/*
 * A051959  Expansion of (1+6*x)/((1-2*x-x^2)*(1-x)^2).
 * https://oeis.org/A051959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + (7*n+1), a(0) = 1, a(1) = 10
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 22, u64 n <= 47, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;

seq_t a(idx_t n) {
    /* Handle the first two terms explicitly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;

    /* Initialise the cache on first call. */
    if (cache_size == 0) {
        cache_size = 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)10;
    }

    /* Extend the cache if necessary. */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return (seq_t)0; /* In case of allocation failure. */
        cache = new_cache;
        /* Compute new terms iteratively. */
        for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
            seq_t prev1 = cache[i - 1];
            seq_t prev2 = cache[i - 2];
            seq_t linear = (seq_t)7 * (seq_t)i + (seq_t)1;
            cache[i] = ((seq_t)2 * prev1) + prev2 + linear;
        }
        cache_size = new_size;
    }

    return cache[n];
}
