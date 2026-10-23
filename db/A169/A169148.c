/*
 * A169148  Number of reduced words of length n in Coxeter group on 39
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^26 = I.
 * https://oeis.org/A169148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the recurrence
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   a(0) = 1                                                          */
/*   for 1 ≤ n ≤ 25  :  a(n) = 37·∑_{k=1}^{n} a(n-k) + 2              */
/*   for n = 26      :  a(26) = 37·∑_{k=1}^{25} a(26-k) – 703·a(0) + 1 */
/*   for n ≥ 27      :  a(n) = 37·∑_{k=1}^{25} a(n-k) – 703·a(n-26)    */
/* These are obtained from the rational generating function           */
/*   (t^26 + 2t^25 + … + 2t + 1) / (703t^26 – 37t^25 – … – 37t + 1)   */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_capacity = 0;
    static size_t cache_len = 0;

    /* init cache with a(0) = 1 */
    if (cache_len == 0) {
        cache_capacity = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_capacity);
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* extend cache up to index n */
    if (n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        if (new_len > cache_capacity) {
            cache_capacity = new_len;
            cache = (seq_t *)realloc(cache, cache_capacity * sizeof(seq_t));
        }

        for (idx_t m = cache_len; m <= n; ++m) {
            seq_t sum = (seq_t)0;
            /* sum of the previous up to 25 terms */
            idx_t start = (m > 25 ? m - 25 : (idx_t)0);
            for (idx_t k = start; k < m; ++k) {
                sum += cache[(size_t)k];
            }

            seq_t term = (seq_t)37 * sum;

            if (m >= 26) {
                term -= (seq_t)703 * cache[(size_t)(m - 26)];
            }

            /* add the numerator term p_m */
            if (m <= 25 && m >= 1) {
                term += (seq_t)2;
            } else if (m == 26) {
                term += (seq_t)1;
            }
            /* for m >= 27, p_m = 0, so nothing to add */

            cache[(size_t)m] = term;
        }

        cache_len = new_len;
    }

    return cache[(size_t)n];
}
