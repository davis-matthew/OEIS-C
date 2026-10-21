/*
 * A068766  Generalized Catalan numbers 4*x*A(x)^2 -A(x)+1-3*x=0.
 * https://oeis.org/A068766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 1, for n ≥ 1 a(n+1) = 4 * Σ_{k=0}^{n} a(k) *
 *            a(n-k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the convolution form of the recurrence, which requires only
   multiplication and addition of seq_t values. */
#include <stdlib.h>

static seq_t *cache = NULL;   /* 0‑based array of precomputed terms */
static idx_t cache_len = 0;   /* current number of entries in cache */

seq_t a(idx_t n) {
    /* Initialise cache if needed */
    if (cache_len == 0) {
        cache_len = 2;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 1;
    }

    /* If the requested term is already cached, return it */
    if (n < cache_len)
        return cache[n];

    /* Grow the cache to hold terms up to index n */
    size_t new_len = n + 1;
    cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));

    /* Compute new terms incrementally using the convolution formula */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 0; k < i; ++k) {
            sum += cache[k] * cache[i - 1 - k];
        }
        /* term a(i) = 4 * Σ a(k) * a(i-1-k) */
        cache[i] = 4 * sum;
    }

    cache_len = new_len;
    return cache[n];
}
