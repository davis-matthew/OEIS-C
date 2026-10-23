/*
 * A199494  a(n) = 3*8^n + 1.
 * https://oeis.org/A199494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*8^n + 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for types used in the build */

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 0; i < n; ++i)
        result *= (seq_t)8;
    result = result * (seq_t)3 + (seq_t)1;
    return result;
}
