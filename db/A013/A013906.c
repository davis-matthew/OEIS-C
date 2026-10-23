/*
 * A013906  a(n) = 23^(5*n + 1).
 * https://oeis.org/A013906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 23^(5*n + 1)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 5,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Compute the exponent 5*n + 1 */
    idx_t exp = 5 * n + 1;
    /* Use the provided helper to compute 23^exp */
    return pow_uint(23, exp);
}
