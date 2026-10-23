/*
 * A013769  a(n) = 21^(3*n + 2).
 * https://oeis.org/A013769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21^(3*n + 2)
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 4, u128 n <= 9,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* compute the exponent 3*n + 2 as an unsigned integer */
    idx_t exp = 3 * n + 2;
    /* use the provided power routine, which returns a seq_t value */
    return pow_uint((uint64_t)21, exp);
}
