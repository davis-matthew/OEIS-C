/*
 * A177704  Period 4: repeat [1, 1, 1, 2].
 * https://oeis.org/A177704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = 1 for n mod 4 == 0,1,2; a(n) = 2 for n mod 4 == 3
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute n modulo 4 using bitwise AND (since 4==2^2) */
    return ((n & 3) == 3) ? (seq_t)2 : (seq_t)1;
}
