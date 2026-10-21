/*
 * A213288  Number of 9-length words w over n-ary alphabet such that for every
 *          prefix z of w we have #(z,a_i) = 0 or #(z,a_i) >= #(z,a_j) for all
 *          j>i and #(z,a_i) counts the occurrences of the i-th letter in z.
 * https://oeis.org/A213288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=0}^{8} coeff_k * C(n+8-k, 9) with coeff_k = {1,
 *            152, 499, -290, 6224, 6496, 41203, 52034, 256561}
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 15, u64 n <= 141, u128 n <= 1000,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* a(0) = 0 */
    if (n == 0) return 0;

    static const int coeffs[9] =
        {  1, 152, 499, -290,  6224,  6496,  41203,  52034,  256561 };

    seq_t pos = 0; /* sum of positive contributions */
    seq_t neg = 0; /* sum of absolute values of negative contributions */

    /* iterate over k from 0 to 8, but skip terms where a < 9 */
    for (idx_t k = 0; k < 9; ++k) {
        /* For terms to be nonzero we need n + 8 - k >= 9  => n >= 1 + k */
        if (n <= k) continue;               /* n <= k means a < 9 */

        idx_t a = n + 8 - k;
        seq_t bin = binom(a, 9);            /* C(a, 9) */

        int coeff = coeffs[k];
        if (coeff >= 0)
            pos += (seq_t)coeff * bin;
        else
            neg += (seq_t)(-coeff) * bin;
    }

    /* pos >= neg by construction, as a(n) is nonnegative */
    return pos - neg;
}
