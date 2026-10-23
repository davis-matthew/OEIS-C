/*
 * A009982  Powers of 38.
 * https://oeis.org/A009982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 38^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* pow_uint handles the case n == 0 returning 1, but we keep the guard
       for clarity and speed. */
    if (n == 0) {
        return (seq_t)1;
    }
    return pow_uint(38, n);
}
