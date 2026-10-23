/*
 * A037723  Base 4 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,0,3,1.
 * https://oeis.org/A037723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = a(n-1)*4 + d[(n-1) mod 4], where d = {2,0,3,1}; a(1)=2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint8_t pattern[4] = {2, 0, 3, 1};

seq_t a(idx_t n)
{
    seq_t val = 0;
    for (idx_t i = 0; i < n; ++i) {
        val = val * 4 + pattern[i & 3];
    }
    return val;
}
