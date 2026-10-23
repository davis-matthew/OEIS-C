/*
 * A195464  a(n) = 2^(4*n + 3) + 2*4^n + 3.
 * https://oeis.org/A195464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(4*n+3) + 2^(2*n+1) + 3
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Exponent for 2^(4*n+3) */
    idx_t e1 = 4 * n + 3;
    /* Exponent for 2^(2*n+1) (equals 2*4^n) */
    idx_t e2 = 2 * n + 1;

    seq_t term1 = pow2(e1);
    seq_t term2 = pow2(e2);

    return term1 + term2 + (seq_t)3;
}
