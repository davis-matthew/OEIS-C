/*
 * A168971  Number of reduced words of length n in Coxeter group on 6
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^23 = I.
 * https://oeis.org/A168971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For 1 ≤ n ≤ 22: a(n) = 4·∑_{k=1}^{n} a(n‑k) + 2 For n =
 *            23: a(23) = 4·∑_{k=1}^{22} a(23‑k) – 10·a(0) + 1 For n ≥ 24:
 *            a(n) = 4·∑_{k=1}^{22} a(n‑k) – 10·a(n‑23)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    if (n >= cache_size) {
        /* grow cache to hold values up to n */
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        cache = tmp;
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[0] = 1;
            } else if (i <= 22) {
                seq_t sum = 0;
                for (idx_t k = 1; k <= i; ++k)
                    sum += cache[i - k];
                cache[i] = 4 * sum + 2;
            } else if (i == 23) {
                seq_t sum = 0;
                for (idx_t k = 1; k <= 22; ++k)
                    sum += cache[i - k];
                cache[i] = 4 * sum - 10 * cache[0] + 1;
            } else { /* i >= 24 */
                seq_t sum = 0;
                for (idx_t k = 1; k <= 22; ++k)
                    sum += cache[i - k];
                cache[i] = 4 * sum - 10 * cache[i - 23];
            }
        }
        cache_size = n + 1;
    }
    return cache[n];
}
