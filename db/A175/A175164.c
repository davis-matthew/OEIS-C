/*
 * A175164  a(n) = 16*(2^n - 1).
 * https://oeis.org/A175164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16 * (2^n - 1)
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 60, u128 n <= 124,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* compute 2^n using the provided utility */
    seq_t t = pow2(n);
    /* subtract 1 */
    t -= 1;
    /* multiply by 16 */
    t *= 16;
    return t;
}
