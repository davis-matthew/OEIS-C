/*
 * A122391  Dimension of 2-variable non-commutative harmonics (Hausdorff
 *          derivative). The dimension of the space of non-commutative
 *          polynomials in 2 variables which are killed by all symmetric
 *          differential operators (where for a monomial w, d_{xi} ( w ) = sum
 *          over all subwords of w deleting xi once).
 * https://oeis.org/A122391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for 0 ≤ n < 3, and a(n) = 3 * 2^(n-3) for n ≥ 3
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)1;
    return (seq_t)3 * pow2(n - 3);
}
