/*
 * A044812  Numbers n such that string 9,9 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 100*(n-1) + 99
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers n such that the decimal representation of n contains "99" but n+1 does not. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert index to seq_t first, then perform arithmetic. */
    seq_t sn = (seq_t)n;          /* sn = n */
    seq_t res = 100 * (sn - 1) + 99;
    return res;
}
