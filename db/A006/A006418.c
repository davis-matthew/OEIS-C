/*
 * A006418  Number of loopless rooted planar maps with 5 faces and n vertices
 *          and no isthmuses.
 * https://oeis.org/A006418
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{j=0}^9 coeff[j] * C(n-j+7, 9) derived from G.f. x^2
 *            * P(x) / (x-1)^10
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 30, u64 n <= 398, u128 n <= 1000,
 *            big any n
 * Helpers:   a000707_binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000707_binom.h"


seq_t a(idx_t n) {
    int coeff[] = {1, 28, 134, -92, -482, 980, -879, 440, -120, 14};
    seq_t pos = 0, neg = 0;
    
    for (int j = 0; j < 10; j++) {
        seq_t binom_val = a000707_binom(n - j + 7, 9);
        if (coeff[j] > 0) {
            pos += (seq_t)coeff[j] * binom_val;
        } else if (coeff[j] < 0) {
            neg += (seq_t)(-coeff[j]) * binom_val;
        }
    }
    
    return pos - neg;
}
