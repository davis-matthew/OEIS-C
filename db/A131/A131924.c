/*
 * A131924  Row sums of triangle A131923.
 * https://oeis.org/A131924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n + n^2 + n
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t res = pow2(n);                     /* 2^n */
    seq_t t   = (seq_t)n * ((seq_t)n + 1);   /* n^2 + n */
    res += t;
    return res;
}
