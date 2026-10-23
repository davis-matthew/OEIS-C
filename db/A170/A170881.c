/*
 * A170881  a(0)=0; thereafter a(n) = (3*n+1)*2^(n-2)+1.
 * https://oeis.org/A170881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n-2)*2^(n-3)+1 for n≥3; a(0)=0, a(1)=1, a(2)=3
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;

    /* n ≥ 3 */
    seq_t coeff = 3 * (seq_t)n - (seq_t)2;
    seq_t pow   = pow2(n - 3);
    return coeff * pow + (seq_t)1;
}
