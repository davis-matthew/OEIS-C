/*
 * A158561  a(n) = ((2^n)*((2^n)+1) - (2^(n-1))*((2^(n-1))+1))/2, a(1)=3.
 * https://oeis.org/A158561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((2^n)*(2^n+1) - (2^(n-1))*(2^(n-1)+1))/2, a(1)=3
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A158561, from offset 1 */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined offset, safe default */
    if (n == 1) return (seq_t)3;          /* special first term */

    seq_t pow_n   = pow2(n);               /* 2^n  */
    seq_t pow_nm1 = pow2(n - 1);           /* 2^(n-1) */

    seq_t term1 = pow_n * (pow_n + (seq_t)1);
    seq_t term2 = pow_nm1 * (pow_nm1 + (seq_t)1);

    return (term1 - term2) / (seq_t)2;
}
