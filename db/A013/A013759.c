/*
 * A013759  a(n) = 16^(3n + 2).
 * https://oeis.org/A013759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16^(3n+2) = 256 * 4096^n = 2^(12n+8)
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 4, u128 n <= 9,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute 2^(12n+8) which equals 16^(3n+2) */
    idx_t exp = 12 * n + 8;
    return pow2(exp);
}
