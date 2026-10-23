/*
 * A013857  a(n) = 10^(5*n + 4).
 * https://oeis.org/A013857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^(5*n + 4)
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 3, u128 n <= 6,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Compute 10^(5*n + 4).  The helper pow10 handles arbitrary
       exponent sizes and returns a seq_t value. */
    return pow10(5 * n + 4);
}
