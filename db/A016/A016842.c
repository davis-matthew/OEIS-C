/*
 * A016842  a(n) = (4*n + 3)^6.
 * https://oeis.org/A016842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n + 3)^6
 * Range:     u8 none, u16 n <= 0, u32 n <= 9, u64 n <= 405, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t x = (seq_t)4 * n + 3;
    seq_t result = 1;
    for (int i = 0; i < 6; ++i)
        result *= x;
    return result;
}
