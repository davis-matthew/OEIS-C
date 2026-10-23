/*
 * A024066  a(n) = 6^n - n^4.
 * https://oeis.org/A024066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n - n^4
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* 6^n */
    seq_t six_pow = pow_uint(6, n);

    /* n^4 */
    seq_t n_sq = (seq_t)n * (seq_t)n;   /* n^2 */
    seq_t n_four = n_sq * n_sq;          /* (n^2)^2 = n^4 */

    /* result */
    return six_pow - n_four;
}
