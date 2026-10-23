/*
 * A135378  Main diagonal of "square and add k" array.
 * https://oeis.org/A135378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A[n,n] where A[k,0] = 2 and A[k,m] = A[k,m-1]^2 + k
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation evaluates the diagonal recursively, caching results */

#include <stddef.h>

seq_t a(idx_t n) {
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Allocate cache if needed */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        /* initialize as zero (unused) */
        for (idx_t i = cache_len; i < new_len; ++i) cache[i] = 0;
        cache_len = new_len;
    }

    /* Return cached value if available */
    if (cache[n] != 0) return cache[n];

    /* Compute the diagonal value for this n */
    seq_t v = 2;
    for (idx_t i = 1; i <= n; ++i) {
        v = v * v + (seq_t)n;   /* A[n,i] = A[n,i-1]^2 + n   */
    }

    /* Store and return */
    cache[n] = v;
    return v;
}
