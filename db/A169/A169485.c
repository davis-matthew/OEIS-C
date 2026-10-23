/*
 * A169485  Number of reduced words of length n in Coxeter group on 40
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^33 = I.
 * https://oeis.org/A169485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For n ≥ 1, a(n) = 38·(a(n‑1)+a(n‑2)+…+a(n‑32)) –
 *            741·a(n‑33) + p(n) where p(n) = 2 for 1 ≤ n ≤ 32, p(33)=1,
 *            p(0)=1, and p(n)=0 otherwise
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;        /* array of a[0..cache_len-1] */
    static idx_t cache_len = 0;        /* number of values currently stored */

    if (cache_len <= n) {                   /* extend the cache */
        size_t new_len = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) return 0;           /* allocation failure */
        cache = new_cache;

        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t p;
            if (i == 0 || i == 33) {
                p = 1;
            } else if (i <= 32) {
                p = 2;
            } else {
                p = 0;
            }

            seq_t sum32 = 0;
            for (idx_t k = 1; k <= 32; ++k) {
                if (i >= k) {
                    sum32 = sum32 + cache[i - k];
                }
            }

            seq_t term = 38 * sum32 + p;
            if (i >= 33) {
                term = term - 741 * cache[i - 33];
            }
            cache[i] = term;
        }
        cache_len = (idx_t)(n + 1);
    }
    return cache[n];
}
