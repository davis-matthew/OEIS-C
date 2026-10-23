/*
 * A160841  Expansion of
 *          (1+147*x+1230*x^2+1915*x^3+744*x^4+66*x^5+x^6)/(1-x)^7.
 * https://oeis.org/A160841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1+3*n*(n+1)*(38*n^4+112*n^3+183*n^2+127*n+50)/20
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 29, u64 n <= 1215,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Use an unsigned 128‑bit integer for intermediate values to avoid overflow */
    unsigned __int128 N = (unsigned __int128)n;

    unsigned __int128 N2 = N * N;
    unsigned __int128 N3 = N2 * N;
    unsigned __int128 N4 = N3 * N;

    unsigned __int128 P = 38 * N4 + 112 * N3 + 183 * N2 + 127 * N + 50;

    unsigned __int128 prod = 3 * N * (N + 1) * P;
    unsigned __int128 result = 1 + prod / 20;

    return (seq_t)result;
}
