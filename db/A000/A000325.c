/*
 * A000325  a(n) = 2^n - n.
 * https://oeis.org/A000325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n - n
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t p = 1;
    idx_t i;
    for (i = 0; i < n; i++)
        p += p;
    return p - (seq_t)n;
}
