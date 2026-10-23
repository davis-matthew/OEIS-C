/*
 * A158066  a(n) = 49*n + 1.
 * https://oeis.org/A158066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 49*n + 1
 * Range:     u8 n <= 5, u16 n <= 1337, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    return (seq_t)49 * (seq_t)n + (seq_t)1;
}
