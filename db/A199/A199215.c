/*
 * A199215  a(n) = 4*5^n + 1.
 * https://oeis.org/A199215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*5^n + 1
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/power5.h"

seq_t a(idx_t n)
{
    seq_t p = power5(n);      /* 5^n */
    return p * 4 + 1;          /* 4*5^n + 1 */
}
