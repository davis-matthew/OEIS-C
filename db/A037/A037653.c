/*
 * A037653  Base 4 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,0,2.
 * https://oeis.org/A037653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 4 * a(n-1) + d_n with d_n = 3, 0, 2 repeating (d_1 = 3,
 *            d_2 = 0, d_3 = 2, d_4 = 3, …), and a(0) = 0
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const unsigned int pat[] = {3U, 0U, 2U};

seq_t a(idx_t n)
{
    seq_t acc = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        unsigned int d = pat[(i - 1) % 3];
        /* use 128‑bit intermediate to avoid overflow when seq_t < 64 bits */
        acc = (seq_t)(((__uint128_t)acc * 4u) + d);
    }
    return acc;
}
