/*
 * A013824  a(n) = 2^(5*n + 3).
 * https://oeis.org/A013824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(5*n + 3)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 8; a(n) = 32*a(n-1) for n > 0 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* compute 2^(5*n + 3) */
    return pow2(5*n + 3);
}
