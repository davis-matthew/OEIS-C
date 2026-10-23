/*
 * A037613  Base 6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,3,2.
 * https://oeis.org/A037613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 6*a(n-1) + d(n) where d(n) cycles 1,3,2 for n=1,2,3,
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* seq_t and idx_t are defined by the build system.  No other headers needed. */
seq_t a(idx_t n)
{
    /* Pattern repeats 1,3,2.  Since idx_t is unsigned, use modulo 3. */
    static const unsigned pattern[3] = {1, 3, 2};

    seq_t value = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        unsigned digit = pattern[(i - 1) % 3];
        value = value * 6 + (seq_t)digit;
    }
    return value;
}
