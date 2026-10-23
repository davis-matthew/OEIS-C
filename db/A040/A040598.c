/*
 * A040598  Continued fraction for sqrt(623).
 * https://oeis.org/A040598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult, less
 * Formula:   a(0)=24; for n>0: a(n)=1 if n is odd, 23 if n has exactly one
 *            factor of 2, 48 if n has at least two factors of 2
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalently: multiplicative with a(2)=23, a(2^e)=48 for e≥2, and a(p^e)=1 for odd primes p. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)24;

    /* Count the exponent of 2 in n. */
    idx_t e = 0;
    idx_t m = n;
    while ((m & 1u) == 0) {   /* m is even */
        ++e;
        m >>= 1;
    }

    if (e == 0) return (seq_t)1;      /* n is odd */
    if (e == 1) return (seq_t)23;     /* exactly one factor of 2 */
    return (seq_t)48;                 /* two or more factors of 2 */
}
