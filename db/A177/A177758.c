/*
 * A177758  Number of ways to place 5 nonattacking bishops on an n X n
 *          toroidal board.
 * https://oeis.org/A177758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1/240*(n-4)^2*(n-2)^2*n^2*(2*n^4 -16*n^3 +54*n^2
 *            -108*n+153 +(10*n^2 -60*n +135)*(-1)^n)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 16, u64 n <= 138, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n < 5 the board is too small for 5 non‑attacking bishops. */
    if (n < 5)
        return 0;

    /* Use 128‑bit integers for the intermediate calculation.  */
    unsigned __int128 N  = (unsigned __int128)n;
    unsigned __int128 n4 = N - 4;
    unsigned __int128 n2 = N - 2;

    /* Compute the polynomial parts of the formula. */
    unsigned __int128 base = (unsigned __int128)2 * N * N * N * N
                           - (unsigned __int128)16 * N * N * N
                           + (unsigned __int128)54 * N * N
                           - (unsigned __int128)108 * N
                           + 153;
    unsigned __int128 poly = (unsigned __int128)10 * N * N
                           - (unsigned __int128)60 * N
                           + 135;

    unsigned __int128 inner = (N & 1) ? (base - poly) : (base + poly);

    /* Multiply all factors and divide by 240.  */
    unsigned __int128 num  = n4 * n4 * n2 * n2 * N * N * inner;
    unsigned __int128 res  = num / 240;

    return (seq_t)res;
}
