/*
 * A032444  a(1) = 1, a(2) = 16, a(n) = lcm(48, 2n^2) for n>2.
 * https://oeis.org/A032444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=16, a(n)=lcm(48, 2*n^2) for n>2
 * Range:     u8 n <= 4, u16 n <= 36, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)16;

    seq_t t = (seq_t)n;
    t *= t;          /* n^2          */
    t *= (seq_t)2;   /* 2*n^2        */

    seq_t g = gcd((seq_t)48, t);
    return ((seq_t)48 / g) * t;  /* lcm(48, 2*n^2) */
}
