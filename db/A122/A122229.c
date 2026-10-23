/*
 * A122229  a(n) = A014486(A122228(n)).
 * https://oeis.org/A122229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 0, a(n) = (9*4^n - (n%2?16:24)) / 10 for n >= 1 (since
 *            4^n = 2^(2n), we compute 2^(2n) with pow2)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* 4^n = 2^(2n) */
    seq_t pow4 = pow2((idx_t)(2 * n));

    seq_t num = pow4 * 9;                /* 9*4^n  */
    seq_t sub = (n & 1) ? (seq_t)16 : (seq_t)24; /* subtract 16 if n odd, else 24 */

    return (num - sub) / (seq_t)10;
}
