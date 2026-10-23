/*
 * A017305  a(n) = 10*n + 3.
 * https://oeis.org/A017305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*n + 3
 * Range:     u8 n <= 25, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    return (seq_t)n * (seq_t)10 + (seq_t)3;
}
