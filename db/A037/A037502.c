/*
 * A037502  Base 9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,0,2.
 * https://oeis.org/A037502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-1) * 9 + d(n), where d(n) cycles through 1,0,2
 *            starting at n=1; a(1) = 1. This builds the base‑9 number whose
 *            digits are the first n terms of the periodic sequence 1,0,2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t val = 0;
    static const uint8_t digit[3] = {1, 0, 2};  /* periodic digits */
    for (idx_t i = 0; i < n; ++i)
    {
        val = val * 9 + digit[i % 3];
    }
    return val;
}
