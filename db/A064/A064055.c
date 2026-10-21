/*
 * A064055  Ninth column of quadrinomial coefficients.
 * https://oeis.org/A064055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*C(n+3,3) + 19*C(n+3,4) + 30*C(n+3,5) + 21*C(n+3,6) +
 *            7*C(n+3,7) + C(n+3,8) (n >= 0) where C(n,k) is the binomial
 *            coefficient binomial(n,k). This follows from a(n) =
 *            A008287(n+3,8) = binomial(n+3,3) *
 *            (n^5+46*n^4+875*n^3+7118*n^2+23880*n+20160)/(8!/3!)
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    idx_t m = n + 3;           /* we need binomial(m, k) for k=3..8 */
    seq_t result = 0;

    /* C(m,3) is always defined because m>=3 for n>=0 */
    seq_t c3 = binomial(m, 3);
    result += (seq_t)3 * c3;

    if (m >= 4) {
        seq_t c4 = binomial(m, 4);
        result += (seq_t)19 * c4;
    }
    if (m >= 5) {
        seq_t c5 = binomial(m, 5);
        result += (seq_t)30 * c5;
    }
    if (m >= 6) {
        seq_t c6 = binomial(m, 6);
        result += (seq_t)21 * c6;
    }
    if (m >= 7) {
        seq_t c7 = binomial(m, 7);
        result += (seq_t)7 * c7;
    }
    if (m >= 8) {
        seq_t c8 = binomial(m, 8);
        result += c8;          /* coefficient is 1 */
    }

    return result;
}
