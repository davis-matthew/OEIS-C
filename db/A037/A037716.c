/*
 * A037716  Base 4 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,0,1,3.
 * https://oeis.org/A037716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = base‑4 number formed from the first n digits of the
 *            repeating pattern 2,0,1,3,2,0,1,3,… (offset 1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t result = 0;
    static const seq_t pattern[4] = {2, 0, 1, 3};
    for (idx_t i = 1; i <= n; ++i) {
        result = result * 4 + pattern[(unsigned)((i - 1) & 3u)];
    }
    return result;
}
