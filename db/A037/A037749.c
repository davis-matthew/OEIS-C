/*
 * A037749  Base 9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,3,0,1.
 * https://oeis.org/A037749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number whose base‑9 representation is the first n * terms
 *            of the periodic sequence 2,3,0,1,2,3,0,1,… * * Implementation:
 *            a(1)=2; for n>1 a(n)=9*a(n-1)+digit_{n-1} * where digit_{k} =
 *            {2,3,0,1} repeating every 4 positions
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for n<1, return 0 */
    seq_t val = (seq_t)0;
    for (idx_t i = 1; i <= n; ++i) {
        val = val * 9;                 /* shift left one base‑9 digit */
        switch ((i - 1) & 3) {          /* (i‑1) mod 4 */
            case 0: val += 2; break;   /* 2 */
            case 1: val += 3; break;   /* 3 */
            case 2: val += 0; break;   /* 0 – no change */
            case 3: val += 1; break;   /* 1 */
        }
    }
    return val;
}
