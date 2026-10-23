/*
 * A013846  a(n) = 8^(5*n + 1).
 * https://oeis.org/A013846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^(5*n + 1)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 4, u128 n <= 8,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* compute exponent 5*n + 1 */
    idx_t exp = 5 * n + 1;
    return pow_uint(8ULL, exp);
}
