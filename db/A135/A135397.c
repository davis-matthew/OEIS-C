/*
 * A135397  a(n) = 2^n * 3^(n^2).
 * https://oeis.org/A135397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n * 3^(n^2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    idx_t sq = n * n;                       /* n^2 */
    seq_t pow2 = seq_pow((seq_t)2, n);       /* 2^n   */
    seq_t pow3 = seq_pow((seq_t)3, sq);      /* 3^(n^2) */
    return seq_mul(pow2, pow3);              /* product */
}
