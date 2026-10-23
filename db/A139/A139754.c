/*
 * A139754  a(n) = floor(n*2*(3^n-2^n)/2^n).
 * https://oeis.org/A139754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(2*n*((3^n)-(2^n))/2^n)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;         /* cast index to seq_t */
    seq_t pow3 = big_pow((seq_t)3, sn);   /* compute 3^n   */
    seq_t pow2 = big_pow((seq_t)2, sn);   /* compute 2^n   */
    seq_t diff = pow3 - pow2;            /* 3^n - 2^n   */
    seq_t twon = sn * (seq_t)2;          /* 2*n        */
    seq_t numer = twon * diff;           /* 2*n*(3^n-2^n) */
    seq_t denom = pow2;                  /* 2^n        */
    seq_t result = numer / denom;        /* floor division */
    return result;
}
