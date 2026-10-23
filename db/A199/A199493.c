/*
 * A199493  a(n) = 2*8^n + 1.
 * https://oeis.org/A199493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*8^n + 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Compute 8^n as a seq_t value. */
    seq_t pow8 = pow_uint((uint64_t)8, n);
    /* a(n) = 2*8^n + 1 */
    return pow8 * (seq_t)2 + (seq_t)1;
}
