/*
 * A190991  a(n) = 13*n + 1.
 * https://oeis.org/A190991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*n + 1
 * Range:     u8 n <= 19, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    return (seq_t)n * (seq_t)13 + (seq_t)1;
}
