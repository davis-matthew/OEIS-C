/*
 * A185968  a(n) = n^2*2^(n^2-1).
 * https://oeis.org/A185968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2*2^(n^2-1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* n^2 in seq_t */
    seq_t nn = (seq_t)n * (seq_t)n;

    /* 2^(n^2-1) */
    seq_t power = pow2((idx_t)(nn - 1));

    /* result : n^2 * 2^(n^2-1) */
    return nn * power;
}
