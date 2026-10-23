/*
 * A013882  a(n) = 17^(5*n + 1).
 * https://oeis.org/A013882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17^(5*n + 1)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 6,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* 17^5 = 1419857 */
    const seq_t small_base = (seq_t)1419857;
    const seq_t pow = pow_int(small_base, n);
    return (seq_t)17 * pow;
}
