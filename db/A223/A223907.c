/*
 * A223907  Poly-Cauchy numbers of the second kind hat c_4^(-n).
 * https://oeis.org/A223907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*2^(1+n)+3*2^(1+2*n)+11*3^n+5^n
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* 3 * 2^(1+n) */
    seq_t term1 = 3 * pow_uint(2, n + 1);

    /* 3 * 2^(1+2*n) */
    idx_t exp2 = 1 + (n << 1);
    seq_t term2 = 3 * pow_uint(2, exp2);

    /* 11 * 3^n */
    seq_t term3 = 11 * pow_uint(3, n);

    /* 5^n */
    seq_t term4 = pow_uint(5, n);

    return term1 + term2 + term3 + term4;
}
