/*
 * A040077  Continued fraction for sqrt(87).
 * https://oeis.org/A040077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = 9 if n==0; 3 if n is odd; 18 if n is even and n>0
 * Range:     u8 n <= 8928, u16 n <= 8928, u32 n <= 8928, u64 n <= 8928,
 *            u128 n <= 8928, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)9;
    return (n & 1) ? (seq_t)3 : (seq_t)18;
}
