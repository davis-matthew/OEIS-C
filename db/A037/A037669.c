/*
 * A037669  Base 6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,2,0.
 * https://oeis.org/A037669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=3; for n>1, a(n)=a(n-1)*6 + {3,2,0}[ (n-1) mod 3 ]
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* Offset starts at 1, but guard against 0 */
    seq_t val = 3;                   /* a(1) */
    if (n == 1) return val;
    static const unsigned char pattern[3] = {3, 2, 0};
    for (idx_t i = 2; i <= n; ++i)
    {
        val = val * 6 + pattern[(i - 1) % 3];
    }
    return val;
}
