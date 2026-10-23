/*
 * A037687  Decimal expansion of a(n) is given by the first n terms of the
 *          periodic sequence with initial period 1,0,3,2.
 * https://oeis.org/A037687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10 * a(n‑1) + d(n) where d(n) is the n-th digit of the
 *            periodic sequence 1,0,3,2. That is, a(n) is the integer whose
 *            decimal representation consists of the first n terms of the
 *            period
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Periodic digits: 1,0,3,2 repeating */
    static const int pattern[4] = {1, 0, 3, 2};

    seq_t value = 0;
    idx_t i;
    for (i = 1; i <= n; ++i) {
        /* Append the next digit */
        value = value * 10 + pattern[(int)((i - 1) & 3)];
    }
    return value;
}
