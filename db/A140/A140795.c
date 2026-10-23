/*
 * A140795  We consider how many ways there are of coloring a square grid, n X
 *          n, using just two colors, black & white say. If the resulting grid
 *          has rotational symmetry of order two only, then the number of
 *          different grids is given by this sequence. None of these counted
 *          are the images of any of the others...
 * https://oeis.org/A140795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(2m+1) = 2^(2*m^2 + 2*m - 1) - 2^(m^2 - 1)*(2^(2*m + 1) + 2^m)
 *            + 2^(m*(m + 3)/2); a(2m) = (2^(2*m^2) - 2^(m^2)*(2^m + 2) +
 *            2^((m^2 + m + 2)/2))/4
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n <= 2) return 0;

    if (n & 1) { /* odd n = 2m+1 */
        idx_t m    = (n - 1) / 2;
        idx_t m2   = m * m;
        idx_t exp1 = 2 * m2 + 2 * m - 1;
        seq_t term1 = pow2(exp1);

        idx_t exp3 = m * (m + 3) / 2;
        seq_t term3 = pow2(exp3);

        seq_t term2_partial = pow2(m2 - 1);
        seq_t term2_sum     = pow2(2 * m + 1) + pow2(m);
        seq_t term2         = term2_partial * term2_sum;

        seq_t res = term1 + term3;
        res = res - term2;
        return res;
    } else { /* even n = 2m */
        idx_t m    = n / 2;
        idx_t m2   = m * m;
        idx_t exp1 = 2 * m2;
        seq_t term1 = pow2(exp1);

        idx_t exp3 = (m2 + m + 2) / 2;
        seq_t term3 = pow2(exp3);

        seq_t term2_partial = pow2(m2);
        seq_t term2_sum     = pow2(m) + 2;
        seq_t term2         = term2_partial * term2_sum;

        seq_t res = term1 + term3;
        res = res - term2;
        res = res / 4;
        return res;
    }
}
