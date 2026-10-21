/*
 * A202097  Number of (n+2) X 7 binary arrays avoiding patterns 001 and 011 in
 *          rows and columns.
 * https://oeis.org/A202097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(alpha+4,4)*C(alpha+3,3)*C(beta+4,4)*C(beta+3,3) where
 *            alpha = ⌊(n+3)/2⌋, beta = ⌊(n+2)/2⌋
 * Range:     u8 none, u16 n <= 2, u32 n <= 13, u64 n <= 90, u128 n <= 210,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* E = 4, O = 3 for width w = 7. */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;        /* undefined for offset 1 */
    idx_t alpha = (n + 3) / 2;          /* floor((n+3)/2) */
    idx_t beta  = (n + 2) / 2;          /* floor((n+2)/2) */

    seq_t res = (seq_t)1;
    res *= binomial(alpha + 4, 4);
    res *= binomial(alpha + 3, 3);
    res *= binomial(beta + 4, 4);
    res *= binomial(beta + 3, 3);
    return res;
}
