/*
 * A037488  Base 3 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,1.
 * https://oeis.org/A037488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (7*3^n-(-1)^n-6)/8 (n ≥ 1)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Compute 3^n first */
    seq_t p = pow3(n);

    /* 7 * 3^n */
    seq_t num = p * (seq_t)7;

    /* Subtract (-1)^n + 6:
       when n is even, (-1)^n = 1  → subtract 7
       when n is odd,  (-1)^n = -1 → subtract 5   */
    seq_t sub = (n % 2 == 0) ? (seq_t)7 : (seq_t)5;

    num -= sub;

    /* Divide by 8 */
    return num / (seq_t)8;
}
