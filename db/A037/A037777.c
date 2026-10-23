/*
 * A037777  Base 9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,1,0,2.
 * https://oeis.org/A037777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=3; for n>1, a(n)=9*a(n-1)+d_n  where d_n = {3,1,0,2}[(n-1) mod 4]
 * This builds the base‑9 number whose first n digits are the periodic
 * sequence 3,1,0,2,3,1,0,2,...
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    static const unsigned pat[4] = {3, 1, 0, 2};
    seq_t val = 0;
    idx_t i;

    if (n == 0)
        return 0;           /* safety for out‑of‑range calls */

    /* Build the number digit by digit */
    for (i = 1; i <= n; ++i)
    {
        val = val * 9 + pat[(i - 1) & 3];
    }

    return val;
}
