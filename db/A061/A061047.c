/*
 * A061047  Numerator of 1/49 - 1/n^2.
 * https://oeis.org/A061047
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of (1/49 – 1/n^2) reduced to lowest terms, i.e.
 *            a(n) = (n^2 – 49) / gcd(n^2 – 49, 49·n^2)
 * Range:     u8 n <= 9, u16 n <= 36, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The build supplies seq_t, idx_t, and a compatible gcd function. */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 7)       /* the offset of the OEIS sequence is 7 */
        return 0;    /* undefined for indices below the offset */

    seq_t nn   = (seq_t)n;
    seq_t num  = nn * nn - (seq_t)49;           /* n^2 – 49          */
    seq_t denom = nn * nn;                     /* n^2               */
    denom *= (seq_t)49;                        /* 49·n^2            */
    seq_t g = gcd(num, denom);                 /* cancel common factors */
    return num / g;                            /* reduced numerator  */
}
