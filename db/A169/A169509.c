/*
 * A169509  Number of reduced words of length n in Coxeter group on 16
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^34 = I.
 * https://oeis.org/A169509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(0) = 1, a(1) = 16, a(2..33) = 2 +
 *            14*sum_{i=1}^{min(33,n)} a(n-i), a(34) = 1 + 14*sum_{i=1}^{33}
 *            a(34-i) - 105*a(0), a(n) = 14*sum_{i=1}^{33} a(n-i) -
 *            105*a(n-34) for n >= 35
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initial compute up to index 34 */
    if (cache == NULL) {
        cache_len = 35;                       /* indices 0..34 */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        for (idx_t m = 0; m <= 34; ++m) {
            seq_t val = (m == 0) ? (seq_t)1 : ((m == 34) ? (seq_t)1 : (seq_t)2);
            idx_t upto = (m <= 33) ? m : 33;
            for (idx_t k = 1; k <= upto; ++k) {
                val += (seq_t)14 * cache[m - k];
            }
            if (m == 34) {
                val -= (seq_t)105 * cache[0];
            }
            cache[m] = val;
        }
    }

    /* extend cache if required */
    if (n >= cache_len) {
        idx_t old_len = cache_len;
        cache_len = n + 1;
        cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
        for (idx_t m = old_len; m <= n; ++m) {
            seq_t val = 0;
            for (idx_t i = 1; i <= 33; ++i) {
                val += (seq_t)14 * cache[m - i];
            }
            val -= (seq_t)105 * cache[m - 34];
            cache[m] = val;
        }
    }

    return cache[n];
}
