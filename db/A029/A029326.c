/*
 * A029326  Expansion of 1/((1-x^4)*(1-x^5)*(1-x^6)*(1-x^7)).
 * https://oeis.org/A029326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^3+33*n^2+384*n+3024)/5040 - (n mod 2)*n/48 +
 *            ((3*n^3+4*n^2+2*n+2) mod 5)/5)
 * Range:     u8 n <= 97, u16 n <= 680, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Use 128‑bit integers for intermediate calculations to avoid overflow */
    unsigned __int128 nn  = (unsigned __int128)n;
    unsigned __int128 nn2 = nn * nn;
    unsigned __int128 nn3 = nn2 * nn;

    /* part1 = n^3 + 33*n^2 + 384*n + 3024 */
    unsigned __int128 part1 = nn3
                            + (unsigned __int128)33 * nn2
                            + (unsigned __int128)384 * nn
                            + 3024ULL;

    /* Numerator of the rational expression:
       5*part1 - 525*((n mod 2)*n) + 5040*(((3*n^3+4*n^2+2*n+2) mod 5)) */
    unsigned __int128 num = 5 * part1;

    unsigned __int128 sub = ((n & 1ULL) * n) * 525ULL;   /* (n mod 2)*n*525 */
    num -= sub;

    /* Compute ((3*n^3+4*n^2+2*n+2) mod 5) */
    unsigned __int128 tmp = (unsigned __int128)3 * nn3
                          + (unsigned __int128)4 * nn2
                          + (unsigned __int128)2 * nn
                          + 2ULL;
    unsigned long long rem = (unsigned long long)(tmp % 5ULL);
    num += (unsigned __int128)rem * 5040ULL;

    /* Final result: floor(num / 25200) */
    unsigned long long result = (unsigned long long)(num / 25200ULL);
    return (seq_t)result;
}
