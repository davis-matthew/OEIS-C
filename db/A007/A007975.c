/*
 * A007975  Number of permutations that are 3 "block reversals" away from
 *          12...n.
 * https://oeis.org/A007975
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 + 53*C(n-4,1) + 281*C(n-4,2) + 559*C(n-4,3) +
 *            506*C(n-4,4) + 210*C(n-4,5) + 35*C(n-4,6) for n >= 4 (degree-6
 *            polynomial; sixth differences are constantly 35)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    static const int coef[7] = {2, 53, 281, 559, 506, 210, 35};
    idx_t m;
    seq_t c, sum;
    int k;
    if (n < 4) return 0;
    m = n - 4;
    sum = 0;
    c = 1; /* c = C(m, k) */
    for (k = 0; k <= 6; k++) {
        if (k > 0) {
            if (m < (idx_t)k) break;
            c = c * (seq_t)(m - (idx_t)(k - 1)) / (seq_t)k;
        }
        sum += c * (seq_t)coef[k];
    }
    return sum;
}
