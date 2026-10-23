/*
 * A159357  Number of n X n arrays of squares of integers with every
 *          (n-2)X(n-2) subblock summing to 4.
 * https://oeis.org/A159357
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{j=0}^8 c_j * binom(n-3-j+8, 8) for n ≥ 4.
 *            Coefficients: c = {17, -140, 3718, 90453, 347157, 218190, -9588,
 *            -3511, 224}
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 14, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 4) return 0;            /* values below the offset are unspecified */
    idx_t k = n - 3;                /* we need coefficient r(k) = coeff · binomial */

    const long long coeff[9] = {
        17, -140, 3718, 90453, 347157,
        218190, -9588, -3511, 224
    };

    seq_t sum_pos = 0;
    seq_t sum_neg = 0;

    for (int j = 0; j < 9; ++j) {
        if (k < (idx_t)j) continue;            // binomial is zero for k < j
        idx_t m = k - (idx_t)j + 8;            /* m ≥ 8, binomial will be C(m,8) */

        /* Compute binomial coefficient C(m,8) iteratively. */
        seq_t binom = 1;
        for (int i = 1; i <= 8; ++i) {
            binom = binom * (seq_t)(m - i + 1) / (seq_t)i;
        }

        seq_t term = binom * (seq_t)((coeff[j] >= 0) ? coeff[j] : -coeff[j]);

        if (coeff[j] >= 0)
            sum_pos += term;
        else
            sum_neg += term;
    }

    return sum_pos - sum_neg;          /* sum_pos ≥ sum_neg for all n ≥ 4 */
}
