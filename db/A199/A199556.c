/*
 * A199556  a(n) = 9*8^n + 1.
 * https://oeis.org/A199556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*8^n + 1 Recurrence: a(0) = 10, a(n) = 8*a(n-1) - 7 (n
 *            >= 1)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    seq_t val = 10; /* a(0) */
    for (idx_t i = 0; i < n; ++i)
    {
        val = mul_small(val, 8);
        val -= 7;
    }
    return val;
}
