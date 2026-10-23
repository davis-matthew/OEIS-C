/*
 * A158686  a(n) = 64*n^2 + 1.
 * https://oeis.org/A158686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 64*n^2 + 1
 * Range:     u8 n <= 1, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    return ns * ns * (seq_t)64 + (seq_t)1;
}
