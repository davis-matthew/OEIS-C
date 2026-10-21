/*
 * A057316  Coordination sequence T3 for Zeolite Code SAV.
 * https://oeis.org/A057316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence derived from the * rational
 *            generating function * * G(x) = (1 + x) * (1 + x² + x³ + 2x⁴ + x⁵
 *            + 5x⁷ + 2x⁸ + x⁹ + x¹⁰ * + 7x¹¹ + x¹² + x¹³ + x¹⁴ + 7x¹⁵ - 2x¹⁶
 *            + 3x¹⁷ + 3x¹⁹ * - x²⁰ + 2x²¹) / * ((1 - x)³ (1 - x + x²) (1 + x
 *            + x²) (1 - x² + x⁴) * (1 - x + x² - x³...
 * Range:     u8 n <= 11, u16 n <= 192, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* Coefficients of the denominator polynomial Q(x), degree 23 */
static const int Q[24] = {
    1,  -3,   4,  -4,   5,  -7,   8,  -8,   9, -11,
   12, -12,  12, -12,  11,  -9,   8,  -8,   7,  -5,
    4,  -4,   3,  -1
};

/* Coefficients of the numerator polynomial N(x), degree 22 */
static const int N[23] = {
    1, 1, 1, 2, 3, 3, 1, 5, 7, 3,
    2, 8, 8, 2, 2, 8, 5, 1, 3, 3,
    2, 1, 2
};

seq_t a(idx_t n)
{
    /* Cached values of the sequence */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Allocate or grow cache to include index n */
    if ((size_t)n + 1 > cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)  /* allocation failure – return 0 as safe fallback */
            return (seq_t)0;
        cache = tmp;

        /* Compute new terms */
        for (size_t i = cache_len; i < new_len; ++i) {
            seq_t sum_pos = 0;   /* Σ Q_k > 0  * a(i‑k)  */
            seq_t sum_neg = 0;   /* Σ (-Q_k) * a(i‑k) for Q_k < 0 */

            /* Only terms with k ≤ i and k ≤ 23 contribute */
            size_t max_k = (i < 23) ? i : 23;
            for (int k = 1; k <= (int)max_k; ++k) {
                int q = Q[k];
                if (q < 0) {
                    sum_neg += (seq_t)(-q) * cache[i - k];
                } else if (q > 0) {
                    sum_pos += (seq_t) q * cache[i - k];
                }
            }

            if (i < 23) {
                cache[i] = (seq_t)N[i] + sum_neg - sum_pos;
            } else {               /* For n ≥ 23, right‑hand side is 0 */
                cache[i] = sum_neg - sum_pos;
            }
        }
        cache_len = new_len;
    }

    return cache[n];
}
