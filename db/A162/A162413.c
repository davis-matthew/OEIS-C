/*
 * A162413  Number of reduced words of length n in the Weyl group D_44.
 * https://oeis.org/A162413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in (1-x^44)∏_{i=1}^{43}(1-x^{2i}) /
 *            (1-x)^44
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cached values of a(k) for k < cache_size */
    static idx_t cache_size = 0;     /* number of cached values */

    if (n >= cache_size) {
        idx_t new_size = n + 1;

        seq_t *new_cache = (seq_t*)calloc(new_size, sizeof(seq_t));
        seq_t *num_coeff  = (seq_t*)calloc(new_size, sizeof(seq_t));
        seq_t *binom_series = (seq_t*)calloc(new_size, sizeof(seq_t));

        /* numerator polynomial: (1 - x^44) * ∏_{i=1}^{43} (1 - x^{2i})   */
        num_coeff[0] = 1;                     /* starts as 1 */
        /*  factor (1 - x^44) */
        idx_t k = 44;
        if (k < new_size) {
            for (idx_t pos = new_size - 1 - k; ; --pos) {
                num_coeff[pos + k] -= num_coeff[pos];
                if (pos == 0) break;
            }
        }
        /*  factors (1 - x^{2i}) for i = 1 .. 43 */
        for (idx_t i = 1; i <= 43; ++i) {
            k = 2 * i;
            if (k >= new_size) break;
            for (idx_t pos = new_size - 1 - k; ; --pos) {
                num_coeff[pos + k] -= num_coeff[pos];
                if (pos == 0) break;
            }
        }

        /* binomial series: coefficient of x^t in (1-x)^(-44) is binom(43+t, t) */
        for (idx_t t = 0; t < new_size; ++t) {
            binom_series[t] = binom(43 + t, t);
        }

        /* convolution: a(n) = Σ_{k=0}^{n} num_coeff[k] * binom_series[n-k] */
        for (idx_t kidx = 0; kidx < new_size; ++kidx) {
            seq_t coef = num_coeff[kidx];
            if (coef == 0) continue;
            for (idx_t t = 0; kidx + t < new_size; ++t) {
                new_cache[kidx + t] += coef * binom_series[t];
            }
        }

        /* free temporaries and replace cache */
        free(num_coeff);
        free(binom_series);
        free(cache);
        cache = new_cache;
        cache_size = new_size;
    }
    return cache[n];
}
