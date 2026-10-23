/*
 * A105749  Number of ways to use the elements of {1,...,k}, 0 <= k <= 2n,
 *          once each to form a sequence of n sets, each having 1 or 2
 *          elements.
 * https://oeis.org/A105749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n-1)*a(n-1) + n*(n-1)*a(n-2), a(0) = 1, a(1) = 2.
 *            The function memoises previously computed values to ensure that
 *            calls with arbitrary n run in O(n) time overall
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache with first two terms */
    if (cache == NULL) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* out‑of‑memory, unlikely */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache_size = 2;
    }

    /* extend cache if necessary */
    if (n >= cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache,
                                            (size_t)(n + 1) * sizeof(seq_t));
        if (new_cache == NULL) return (seq_t)0;   /* out‑of‑memory, unlikely */
        cache = new_cache;
        /* compute missing values */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) { cache[0] = (seq_t)1; continue; }
            if (i == 1) { cache[1] = (seq_t)2; continue; }
            seq_t term1 = (seq_t)i * (seq_t)(2 * i - 1);
            term1 *= cache[i - 1];
            seq_t term2 = (seq_t)i * (seq_t)(i - 1);
            term2 *= cache[i - 2];
            cache[i] = term1 + term2;
        }
        cache_size = n + 1;
    }

    return cache[n];
}
