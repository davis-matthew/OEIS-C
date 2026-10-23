/*
 * A170149  Number of reduced words of length n in Coxeter group on 44
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^38 = I.
 * https://oeis.org/A170149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 42*a(n-1) + 42*a(n-2) + … + 42*a(n-37) - 903*a(n-38) +
 *            b_n where b_0 = b_38 = 1, b_k = 2 for 1 ≤ k ≤ 37, and b_k = 0
 *            for k > 38. The sequence starts with a(0)=1, a(1)=44, a(2)=1892,
 *            … and is computed recursively
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* grow the cache to contain indices up to n */
    if (cache_size <= n) {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);
        cache = new_cache;

        /* compute new terms */
        for (idx_t i = cache_size; i <= n; ++i) {
            /* start with the numerator contribution b_i */
            seq_t term;
            if (i == 0 || i == 38)         term = (seq_t)1;
            else if (i <= 37)              term = (seq_t)2;
            else                            term = (seq_t)0;

            /* add 42 * a(i-j) for j = 1 … min(37, i) */
            for (idx_t j = 1; j <= 37 && j <= i; ++j) {
                term += (seq_t)42 * cache[i - j];
            }

            /* subtract 903 * a(i-38) for i ≥ 38 */
            if (i >= 38) {
                term -= (seq_t)903 * cache[i - 38];
            }

            cache[i] = term;
        }

        cache_size = n + 1;
    }

    return cache[n];
}
