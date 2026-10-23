/*
 * A170313  Number of reduced words of length n in Coxeter group on 16
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^42 = I.
 * https://oeis.org/A170313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = b(n) + 14 * Σ_{i=1}^{41} a(n-i) – 105 * a(n-42) where
 *            b(n) = 1 for n = 0 or n = 42, b(n) = 2 for 1 ≤ n ≤ 41, b(n) = 0
 *            otherwise
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;      /* next index to compute */

    if (n >= cache_len) {
        /* grow the cache to accommodate indices up to n */
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();          /* memory allocation failed */
        cache = tmp;

        /* compute new values, starting from the previous size */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t sum = 0;
            /* sum of at most 41 previous terms */
            for (idx_t j = 1; j <= 41 && i >= j; ++j) {
                sum += cache[i - j];
            }
            seq_t val = sum * 14;                  /* 14 * Σ a(i-j) */
            if (i >= 42) {
                val -= cache[i - 42] * 105;        /* –105 * a(i-42) */
            }
            seq_t b = 0;
            if (i <= 42) {
                b = (i == 0 || i == 42) ? (seq_t)1 : (seq_t)2;
            }
            cache[i] = val + b;
        }
        cache_len = new_len;
    }
    return cache[n];
}
