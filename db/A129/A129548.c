/*
 * A129548  Measures of entanglement in 3-qbits.
 * https://oeis.org/A129548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n even) => (n^6 + 24*n^5 + 280*n^4 + 1920*n^3 + 7504*n^2
 *            + 14976*n + 11520) / 11520 a(n) = (n odd) => (n^6 + 24*n^5 +
 *            235*n^4 + 1200*n^3 + 3319*n^2 + 4536*n + 2205) / 11520
 * Range:     u8 none, u16 n <= 1, u32 n <= 36, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    seq_t sn   = (seq_t)n;
    seq_t n2   = sn * sn;
    seq_t n3   = n2 * sn;
    seq_t n4   = n3 * sn;
    seq_t n5   = n4 * sn;
    seq_t n6   = n5 * sn;
    seq_t numerator;

    if ((n & 1) == 0)   /* n even */
    {
        numerator = n6
                   + (24 * n5)
                   + (280 * n4)
                   + (1920 * n3)
                   + (7504 * n2)
                   + (14976 * sn)
                   + 11520;
    }
    else                /* n odd */
    {
        numerator = n6
                   + (24 * n5)
                   + (235 * n4)
                   + (1200 * n3)
                   + (3319 * n2)
                   + (4536 * sn)
                   + 2205;
    }
    return numerator / 11520;
}
