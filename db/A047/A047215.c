/*
 * A047215  Numbers that are congruent to {0, 2} mod 5.
 * https://oeis.org/A047215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(5*n/2)
 * Range:     u8 n <= 102, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use integer arithmetic to avoid overflow as much as possible.
       Compute n/2 and n%2 first, then multiply by 5 and add 2 for the odd case. */
    seq_t q = ((seq_t)n) >> 1;      /* q = n / 2            */
    seq_t r = ((seq_t)n) & 1;       /* r = n % 2            */
    return 5 * q + 2 * r;           /* a(n) = 5*q + 2*r     */
}
