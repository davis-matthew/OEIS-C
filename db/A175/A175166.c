/*
 * A175166  a(n) = 64*(2^n - 1).
 * https://oeis.org/A175166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 64*(2^n - 1) = 2^(n+6) - 64
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 26, u64 n <= 58, u128 n <= 122,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* For n = 0 this returns 64*1 - 64 = 0 as required. */
    return pow2(n + 6) - 64;
}
