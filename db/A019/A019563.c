/*
 * A019563  Coordination sequence for C_7 lattice.
 * https://oeis.org/A019563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0}^{min(7,n)} c_k * C(n-k+6,6) where c = {1, 91,
 *            1001, 3003, 3003, 1001, 91, 1}
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 26, u64 n <= 1083,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* Compute binomial coefficient C(m,6) for m >= 6 */
static seq_t binom6(idx_t m) {
    /* numerators: m, m-1, ..., m-5 */
    seq_t nums[6];
    for (int i = 0; i < 6; ++i) {
        nums[i] = (seq_t)(m - i);
    }
    /* denominators: 1,2,3,4,5,6 */
    seq_t dens[6] = { (seq_t)1, (seq_t)2, (seq_t)3,
                      (seq_t)4, (seq_t)5, (seq_t)6 };

    /* Cancel common factors to avoid overflow */
    for (int d = 0; d < 6; ++d) {
        seq_t dval = dens[d];
        if (dval == 1) continue;
        for (int i = 0; i < 6; ++i) {
            seq_t g = gcd(nums[i], dval);
            if (g > 1) {
                nums[i] /= g;
                dval /= g;
                if (dval == 1) break;
            }
        }
    }

    /* Multiply remaining numerators */
    seq_t result = (seq_t)1;
    for (int i = 0; i < 6; ++i) {
        result *= nums[i];
    }
    return result;
}

seq_t a(idx_t n) {
    static const seq_t coeff[8] = {
        (seq_t)1, (seq_t)91, (seq_t)1001, (seq_t)3003,
        (seq_t)3003, (seq_t)1001, (seq_t)91, (seq_t)1
    };

    seq_t total = (seq_t)0;
    for (idx_t k = 0; k < 8 && k <= n; ++k) {
        idx_t m = n - k + 6;          /* m >= 6 always here */
        seq_t bin = binom6(m);
        total += coeff[k] * bin;
    }
    return total;
}
