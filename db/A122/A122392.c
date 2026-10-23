/*
 * A122392  Dimension of 3-variable non-commutative harmonics (Hausdorff
 *          derivative). The dimension of the space of non-commutative
 *          polynomials in 3 variables which are killed by all symmetric
 *          differential operators (where for a monomial w, d_{xi} ( w ) = sum
 *          over all subwords of w deleting xi once).
 * https://oeis.org/A122392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 416*3^(n-6) for n>5, with a(0)=1, a(1)=2, a(2)=5,
 *            a(3)=15, a(4)=46, a(5)=139
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)15;
    if (n == 4) return (seq_t)46;
    if (n == 5) return (seq_t)139;

    // For n > 5: a(n) = 416 * 3^(n-6)
    seq_t res = pow3(n - 6);          /* 3^(n-6) */
    return res * (seq_t)416;
}
