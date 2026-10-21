/*
 * A053742  Sum of odd numbers in range 10*n to 10*n+9.
 * https://oeis.org/A053742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 25 + 50*n
 * Range:     u8 n <= 4, u16 n <= 1310, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    return (seq_t)25 + (seq_t)50 * (seq_t)n;
}
