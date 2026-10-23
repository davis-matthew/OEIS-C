/*
 * A126898  Coordination sequence for 8-dimensional cyclotomic lattice
 *          Z[zeta_15].
 * https://oeis.org/A126898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A126898: Coordination sequence for 8-dimensional cyclotomic lattice Z[zeta_15].
 * The generating function is
 *   (x^8+7*x^7+28*x^6+79*x^5+130*x^4+79*x^3+28*x^2+7*x+1)/(1-x)^8 .
 * Therefore
 *   a(n) = Σ_{k=0..8} p_k * C(n-k+7, 7)   with p_k = [1,7,28,79,130,79,28,7,1] and
 *   C(m+7,7)=0 when m<0.  This formula is used directly.
 */
#include <stdint.h>

seq_t a(idx_t n) {
    static const int numerator[9] = {1, 7, 28, 79, 130, 79, 28, 7, 1};
    seq_t result = 0;

    for (int k = 0; k <= 8; ++k) {
        if (n < (idx_t)k) continue;           /* no contribution when n < k */
        idx_t m = n - k;                      /* m >= 0 */

        /* Compute binomial coefficient C(m+7,7) multiplicatively to avoid overflow
         * in intermediate factorials.  Use the recurrence C(m+7,7)=C(m+6,6)*(m+7)/7.  */
        seq_t bin = 1;
        for (idx_t i = 1; i <= 7; ++i) {
            bin = bin * (m + i);              /* multiply by numerator term */
            bin = bin / i;                    /* divide by i, guaranteed integer */
        }

        result += (seq_t)numerator[k] * bin;
    }

    return result;
}
