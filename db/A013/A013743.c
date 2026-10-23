/*
 * A013743  a(n) = 8^(3*n + 2).
 * https://oeis.org/A013743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^(3*n + 2)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 6, u128 n <= 13,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Compute the exponent 3*n + 2 safely in idx_t */
    idx_t exp = 3 * n + 2;
    /* Use the provided pow_uint helper to calculate 8^exp in seq_t */
    return pow_uint(8, exp);
}
