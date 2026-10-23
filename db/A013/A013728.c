/*
 * A013728  a(n) = 23^(2*n + 1).
 * https://oeis.org/A013728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 23^(2*n + 1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 13,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* exponent = 2*n + 1 */
    idx_t exp = (idx_t)(2 * n + 1);
    return seq_pow((seq_t)23, exp);
}
