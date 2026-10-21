/*
 * A224289  Number of permutations of length n containing exactly 1 occurrence
 *          of 123 and 2 occurrences of 132.
 * https://oeis.org/A224289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(-8+n)*(-136+70*n-11*n^2+n^3) for n > 3 a(1) = 0, a(2)
 *            = 0, a(3) = 0 (OEIS A224289)
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 26, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n <= 3)
        return 0;

    /* Compute the cubic polynomial in an unsigned form */
    seq_t nn   = (seq_t)n;
    seq_t n2   = nn * nn;           /* n^2 */
    seq_t n3   = n2 * nn;           /* n^3 */

    /* -136 + 70*n - 11*n^2 + n^3 */
    seq_t term = n3 - 11 * n2 + 70 * nn - 136;

    /* Multiply by 2^(n-8) or divide by 2^(8-n) depending on n */
    if (n >= 8) {
        seq_t mult = pow2(n - 8);
        return term * mult;
    } else {
        seq_t denom = pow2(8 - n);
        return term / denom;   /* exact division */
    }
}
