/*
 * A166840  Number of n X 2 1..6 arrays containing at least one of each value,
 *          all equal values connected, rows considered as a single number in
 *          nondecreasing order, and columns considered as a single number in
 *          decreasing order.
 * https://oeis.org/A166840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=0}^{7} c[i] * C(n-3-i+10,10) for n ≥ 3 * where c =
 *            [30, 398, -994, 487, 668, -922, 416, -67], * and a(1)=a(2)=0. C
 *            denotes the binomial coefficient
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n) {
    if (n < 3) return (seq_t)0;

    static const int coeffs[8] = {30, 398, -994, 487, 668, -922, 416, -67};
    seq_t positive = 0, negative = 0;
    idx_t diff = n - 3;   /* n-3 ≥ 0 */

    for (int i = 0; i < 8; ++i) {
        if ((idx_t)i > diff) break;          /* term would use negative binomial */
        idx_t k = diff - (idx_t)i;           /* k = n-3-i  (≥0) */
        seq_t bin = binom(k + 10, 10);       /* C(k+10,10)  */
        seq_t mult = (seq_t)(coeffs[i] >= 0 ? coeffs[i] : -coeffs[i]) * bin;
        if (coeffs[i] >= 0)
            positive += mult;
        else
            negative += mult;
    }

    /* The result is guaranteed non‑negative. */
    return positive - negative;
}
