/*
 * A036826  a(n) = A036800(n)/2.
 * https://oeis.org/A036826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n*(n^2-2*n+3) - 3
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 23, u64 n <= 52, u128 n <= 114,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* 2^n */
    seq_t pow = pow2(n);

    /* n casted to seq_t first */
    seq_t n_seq = (seq_t)n;

    /* ternary part (n^2 - 2*n + 3) */
    seq_t part = n_seq * n_seq - (seq_t)2 * n_seq + (seq_t)3;

    return pow * part - (seq_t)3;
}
