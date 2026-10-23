/*
 * A120846  a(n) = 3^n + 2^n + n.
 * https://oeis.org/A120846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n + 2^n + n
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t pow3 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow3 *= 3;

    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow2 *= 2;

    return pow3 + pow2 + (seq_t)n;
}
