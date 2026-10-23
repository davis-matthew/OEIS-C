/*
 * A020526  a(n) = 6th Euler polynomial evaluated at 2^n.
 * https://oeis.org/A020526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(6*n) - 3*2^(5*n) + 5*2^(3*n) - 3*2^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t p6 = pow2(6 * n);
    seq_t p5 = pow2(5 * n);
    seq_t p3 = pow2(3 * n);
    seq_t p1 = pow2(n);

    seq_t result = p6 - ((seq_t)3) * p5 + ((seq_t)5) * p3 - ((seq_t)3) * p1;
    return result;
}
