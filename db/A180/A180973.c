/*
 * A180973  Numbers n such that 9^10 + n^2 is a square.
 * https://oeis.org/A180973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   a(n) = (3^(9 + n) - 3^(11 - n)) / 2 for 1 ≤ n ≤ 11; otherwise 0
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Return 0 for indices outside the finite sequence */
    if (n < 1 || n > 11)
        return (seq_t)0;

    /* 3^(9+n) and 3^(11-n) are powers of 3; their difference is even */
    seq_t high = pow3(9 + n);   /* 9+n ≤ 20 for n ≤ 11 */
    seq_t low  = pow3(11 - n);

    return (high - low) / 2;
}
