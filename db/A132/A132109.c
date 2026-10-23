/*
 * A132109  a(n) = (2^(n+1) + n^2 + n)/2.
 * https://oeis.org/A132109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^(n+1) + n^2 + n) / 2
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* 2^(n+1) */
    seq_t pow_exp = pow2(n + 1);

    /* n^2 + n */
    seq_t n_sq = (seq_t)n * (seq_t)n;
    seq_t n_total = n_sq + (seq_t)n;

    /* (2^(n+1) + n^2 + n) / 2 */
    seq_t sum = pow_exp + n_total;
    return sum / 2;
}
