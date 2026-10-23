/*
 * A037696  Base 5 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,2,3,0.
 * https://oeis.org/A037696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 5*a(n-1) + d(n), where d(n) = {1,2,3,0}[ (n-1) mod 4 ],
 *            a(0)=0, a(1)=1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    static const uint8_t period[4] = {1, 2, 3, 0};
    seq_t res = 0;
    for (idx_t i = 0; i < n; ++i)
        res = res * 5 + period[i & 3];
    return res;
}
