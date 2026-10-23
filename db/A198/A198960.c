/*
 * A198960  a(n) = 3*9^n-1.
 * https://oeis.org/A198960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*9^n-1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t val = 1;
    for (idx_t i = 0; i < n; ++i)
        val *= 9;
    val = val * 3 - 1;
    return val;
}
