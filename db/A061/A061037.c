/*
 * A061037  Numerator of 1/4 - 1/n^2.
 * https://oeis.org/A061037
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac, nice, easy
 * Formula:   a(n) = (n^2-4) / gcd(n^2-4, 4*n^2)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the reduced numerator of 1/4 - 1/n^2, for n ≥ 2. */

#include <stdint.h>

/* Euclidean gcd for unsigned 128‑bit integers. */
static unsigned __int128 gcd128(unsigned __int128 a, unsigned __int128 b)
{
    while (b)
    {
        unsigned __int128 r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* n is always ≥ 2 by the problem statement. */
    unsigned __int128 nn   = (unsigned __int128) n;
    unsigned __int128 sq   = nn * nn;                     /* n^2          */
    unsigned __int128 num  = sq - (unsigned __int128) 4;  /* n^2 - 4      */
    unsigned __int128 den  = sq * (unsigned __int128) 4;   /* 4 * n^2      */

    unsigned __int128 g    = gcd128(num, den);
    unsigned __int128 res  = num / g;                     /* reduced numerator */

    return (seq_t) res;  /* cast to the desired sequence type */
}
