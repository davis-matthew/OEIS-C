/*
 * A017284  a(n) = (10*n + 1)^4.
 * https://oeis.org/A017284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 1)^4
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 25, u64 n <= 6553,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t m = (seq_t)10 * (seq_t)n + 1;
    seq_t m2 = m * m;
    return m2 * m2;
}
