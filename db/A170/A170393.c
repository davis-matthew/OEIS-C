/*
 * A170393  Number of reduced words of length n in Coxeter group on 48
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^43 = I.
 * https://oeis.org/A170393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (46*(a(n-1)+...+a(n-42)) - a(n-43))/1081 for n≥43. The
 *            initial values a(0)…a(42) are obtained from the generating
 *            function G(t) = (t^43 + 2t^42 + … + 2t + 1)/(1081t^43 – 46t^42 –
 *            … – 46t + 1)
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
    static size_t cache_size = 0;        /* allocated size */
    static size_t computed_max = 0;      /* number of values computed (next index to compute) */

    /* Ensure cache large enough for index n */
    if (cache_size <= (size_t)n) {
        size_t new_size = (size_t)n + 1;
        if (new_size < 64) new_size = 64;          /* at least 64 for safety */
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache_size = new_size;
    }

    /* Compute missing values up to n */
    while (computed_max <= (size_t)n) {
        idx_t i = (idx_t)computed_max;

        if (i < 43) {
            /* Compute a(i) from the numerator poly */
            seq_t sum = (seq_t)((i == 0) ? 1 : 2);
            for (idx_t k = 1; k <= i; ++k) {
                sum += (seq_t)46 * cache[i - k];
            }
            cache[i] = sum;
        } else {
            /* Recurrence for i ≥ 43 */
            seq_t sum = 0;
            for (idx_t k = 1; k <= 42; ++k) {
                sum += cache[i - k];
            }
            seq_t numerator = (seq_t)46 * sum - cache[i - 43];
            cache[i] = numerator / (seq_t)1081;
        }

        ++computed_max;
    }

    return cache[n];
}
