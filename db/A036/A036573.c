/*
 * A036573  Size of maximal triangulation of an n-antiprism with regular
 *          polygonal base.
 * https://oeis.org/A036573
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^2 + 8*n - 16)/4)
 * Range:     u8 n <= 28, u16 n <= 508, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = (nn * nn + 8 * nn - 16) / 4;
    return val;
}
