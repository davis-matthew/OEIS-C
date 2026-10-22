/*
 * A006415  Number of nonseparable toroidal tree-rooted maps with n + 3 edges
 *          and n + 1 vertices.
 * https://oeis.org/A006415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 4*C(n+4,n) + 84*C(n+4,n-1) + 456*C(n+4,n-2) +
 *            996*C(n+4,n-3) + 950*C(n+4,n-4) + 330*C(n+4,n-5) (conjectured
 *            OEIS formula; C(n+4,n-k) = C(n+4,k+4))
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   a006415_choose
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a006415_choose.h"

seq_t a(idx_t n) {
    static const unsigned coef[6] = {4, 84, 456, 996, 950, 330};
    seq_t result = 0;
    idx_t m = n + 4;
    idx_t k;
    for (k = 0; k < 6; k++) {
        if (n < k) break;
        result += (seq_t)coef[k] * a006415_choose(m, k + 4);
    }
    return result;
}
