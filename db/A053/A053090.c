/*
 * A053090  Number of F^3-convex polyominoes on honeycomb lattice with given
 *          semiperimeter.
 * https://oeis.org/A053090
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round( n*(2*n^2 + 3)/144 - (-1)^n*3*n/16 )
 * Range:     u8 n <= 26, u16 n <= 167, u32 n <= 6762, u64 n <= 8322,
 *            u128 n <= 8322, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 3  (a(3) = 1) */

/* This implementation uses the closed‑form expression from the OEIS entry.
   It works for all n >= 3.  For n < 3 the function returns 0; the
   behaviour below the offset is irrelevant for the tests.  */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    /* compute s = (-1)^n : 1 for even n, -1 for odd n */
    int s = (n % 2 == 0) ? 1 : -1;

    /* use 128‑bit intermediate to avoid overflow for all reasonable n */
    unsigned __int128 N = n;
    unsigned __int128 numerator = N * ((unsigned __int128)2 * N * N + 3
                                      - (unsigned __int128)27 * s);

    /* round: floor((numerator/144)+0.5) == floor((numerator+72)/144) */
    numerator += 72;
    unsigned __int128 result = numerator / 144;

    return (seq_t)result;
}
