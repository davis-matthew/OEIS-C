/*
 * A198599  Number of 2nX6 0..3 arrays with values 0..3 introduced in row
 *          major order and each element equal to exactly two horizontal and
 *          vertical neighbors.
 * https://oeis.org/A198599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by the 12‑term linear recurrence a(n) = 27
 *            a(n-1) – 107 a(n-2) – 164 a(n-3) – 1701 a(n-4) + 7073 a(n-5) +
 *            13206 a(n-6) + 22329 a(n-7) – 67473 a(n-8) – 190188 a(n-9) –
 *            20655 a(n‑10) + 60507 a(n‑11) + 177147 a(n‑12) (n ≥ 13) with
 *            initial values a(1) = 2, a(2) = 32, a(3) = 674, a(4)...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   mul_by_pos
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mul_by_pos.h"

/* coefficients of the recurrence: a(n) = Σ coeff[k]*a(n-k-1)  */
static const int32_t coeff[12] = {
    27,   -107,   -164,   -1701,   7073,
   13206, 22329, -67473, -190188, -20655,
    60507, 177147
};

/* initial terms (index 0 → a(1)) */
static const seq_t init[12] = {
    (seq_t)2ULL,
    (seq_t)32ULL,
    (seq_t)674ULL,
    (seq_t)14396ULL,
    (seq_t)309770ULL,
    (seq_t)6671450ULL,
    (seq_t)143724590ULL,
    (seq_t)3096625676ULL,
    (seq_t)66720604082ULL,
    (seq_t)1437592096184ULL,
    (seq_t)30975107870150ULL,
    (seq_t)667406503209962ULL
};

seq_t a(idx_t n)
{
    /* Sequence is defined for n ≥ 1; values for n = 0 are irrelevant. */
    if (n == 0)
        return 0;

    if (n <= 12)
        return init[n - 1];

    /* static cache to memoise computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;     /* number of cached entries */

    /* enlarge cache if necessary */
    if ((size_t)n > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!tmp) {          /* allocation failure */
            return 0;
        }
        cache = tmp;

        /* initialise new entries */
        for (idx_t i = (idx_t)cache_size; i < n; ++i) {
            if (i < 12) {
                cache[i] = init[i];
            } else {
                seq_t sum_pos = 0;
                seq_t sum_neg = 0;
                for (int k = 0; k < 12; ++k) {
                    seq_t prev = cache[i - 1 - k];
                    int32_t c = coeff[k];
                    if (c >= 0) {
                        sum_pos += mul_by_pos(prev,
                                              (unsigned long long)c);
                    } else {
                        sum_neg += mul_by_pos(prev,
                                              (unsigned long long)(-c));
                    }
                }
                cache[i] = sum_pos - sum_neg;
            }
        }
        cache_size = (size_t)n;
    }
    return cache[n - 1];
}
