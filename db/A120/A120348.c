/*
 * A120348  Number of labeled simply-rooted 2-trees with n labeled vertices
 *          (i.e., n+2 vertices altogether; a simply-rooted 2-tree is an
 *          externally rooted 2-tree whose root edge belongs to exactly one
 *          triangle).
 * https://oeis.org/A120348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (5n-2)!/(4n-1)! (equivalently ∏_{k=4n}^{5n-2} k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation stores previously computed values so that later calls
   that request larger n are fast.  For n<1 the function returns 0 (the
   sequence is not defined there). */

#include <stdlib.h>

/* Computes A120348. */
seq_t a(idx_t n)
{
    /* Empty or below offset is undefined – return 0. */
    if (n < 1) {
        return (seq_t)0;
    }

    /* Static cache: cache[0] holds a(1), cache[1] holds a(2), ... */
    static seq_t *cache = NULL;
    static idx_t cached = 0;   /* number of values stored (>=0) */

    /* Return cached value if available. */
    if (n <= cached) {
        return cache[n - 1];
    }

    /* Grow the cache to hold up to n values. */
    idx_t new_cap = (cached == 0) ? 1 : cached << 1;
    while (new_cap < n) {
        new_cap <<= 1;
    }
    if (!cache) {
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    } else {
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    }

    /* Compute and cache the new values. */
    for (idx_t i = cached + 1; i <= n; ++i) {
        seq_t prod = (seq_t)1;
        idx_t start = 4 * i;
        idx_t end   = 5 * i - 2;
        for (idx_t k = start; k <= end; ++k) {
            prod *= (seq_t)k;
        }
        cache[i - 1] = prod;
    }

    cached = n;
    return cache[n - 1];
}
