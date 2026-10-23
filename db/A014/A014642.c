/*
 * A014642  Even octagonal numbers: a(n) = 4*n*(3*n-1).
 * https://oeis.org/A014642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n*(3*n-1)
 * Range:     u8 n <= 4, u16 n <= 74, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return 4 * nn * (3 * nn - 1);
}
