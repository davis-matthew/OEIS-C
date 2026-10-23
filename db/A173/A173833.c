/*
 * A173833  a(n) = 10^n - 3.
 * https://oeis.org/A173833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n - 3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. For n == 0 we simply return 0. */
    if (n == 0)
        return 0;
    seq_t val = pow_uint(10U, n);
    val -= 3;
    return val;
}
