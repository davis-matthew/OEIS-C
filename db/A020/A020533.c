/*
 * A020533  a(n) = 7th Fibonacci polynomial evaluated at 2^n.
 * https://oeis.org/A020533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1+3*2^(1+2*n)+5*16^n+64^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* 2^(1+2n) */
    seq_t term1 = pow2(1 + 2 * n);
    /* 16^n = 2^(4n) */
    seq_t term2 = pow2(4 * n);
    /* 64^n = 2^(6n) */
    seq_t term3 = pow2(6 * n);

    seq_t result = 1;
    result += 3 * term1;
    result += 5 * term2;
    result += term3;
    return result;
}
