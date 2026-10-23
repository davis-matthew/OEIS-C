/*
 * A174309  Value of the polynomial y = (x^(2n-1) + 1)/(x + 1) at the point x
 *          = discriminant of y.
 * https://oeis.org/A174309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((2n-1)^{(2n-1)((2n-1)-2)} + sgn) / ((2n-1)^{(2n-1)-2} +
 *            sgn), where sgn = +1 if n is odd, sgn = -1 if n is even. The
 *            special case n = 1 yields a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* Handle the offset and trivial case */
    if (n == 1) return (seq_t)1;

    /* Convert idx_t to seq_t for calculations */
    seq_t sn = (seq_t)n;

    /* M = 2n - 1 */
    seq_t M = ((seq_t)2 * sn) - (seq_t)1;

    /* e2 = m - 2, e1 = m * (m - 2) */
    seq_t e2 = M - (seq_t)2;
    seq_t e1 = M * e2;

    /* Compute powers of M */
    seq_t pow2 = big_pow(M, e2);   /* M^(m-2)  */
    seq_t pow1 = big_pow(M, e1);   /* M^(m*(m-2))  */

    /* Determine sign based on parity of n */
    seq_t numerator, denominator, result;
    if (n & (idx_t)1) { /* n odd: sgn = +1 */
        numerator = pow1 + (seq_t)1;
        denominator = pow2 + (seq_t)1;
    } else { /* n even: sgn = -1 */
        numerator = pow1 - (seq_t)1;
        denominator = pow2 - (seq_t)1;
    }

    result = numerator / denominator;
    return result;
}
