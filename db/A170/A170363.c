/*
 * A170363  Number of reduced words of length n in Coxeter group on 18
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^43 = I.
 * https://oeis.org/A170363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = c_n + Σ_{k=1}^{min(42,n)} 16*a(n-k) - (n≥43 ? 136*a(n-43)
 *            : 0) with c_0 = 1, c_k = 2 for 1≤k≤42, c_43 = 1, c_k = 0
 *            otherwise. This recurrence follows directly from the generating
 *            function (t^43 + 2*t^42 + … + 2*t + 1) / (136*t^43 – 16*t^42 – …
 *            – 16*t + 1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n >= cache_len) {
        size_t new_len = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) {
            abort(); /* allocation failed */
        }
        cache = new_cache;

        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t sum = 0;

            /* coefficient c_i */
            if (i == 0) {
                sum = 1;
            } else if (i <= 42) {
                sum = 2;
            } else if (i == 43) {
                sum = 1;
            } else {
                sum = 0;
            }

            /* add 16 * a(i-k) for k = 1 … min(42,i) */
            idx_t max_k = (i < 42) ? i : 42;
            for (idx_t k = 1; k <= max_k; ++k) {
                sum += (seq_t)16 * cache[i - k];
            }

            /* subtract 136 * a(i-43) if i ≥ 43 */
            if (i >= 43) {
                sum -= (seq_t)136 * cache[i - 43];
            }

            cache[i] = sum;
        }

        cache_len = n + 1;
    }

    return cache[n];
}
