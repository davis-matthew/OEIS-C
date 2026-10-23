/*
 * A037509  Base 8 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,2,0.
 * https://oeis.org/A037509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = a(n-1) * 8 + d(n), where d(n) = 1 if n%3==1,
 *                 2 if n%3==2, and 0 if n%3==0.
 * This encodes the first n digits of the periodic sequence 1,2,0
 * in base 8.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t val = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t d = (i % 3 == 1) ? 1
               : (i % 3 == 2) ? 2
               : 0;
        val = val * 8 + d;
    }
    return val;
}
