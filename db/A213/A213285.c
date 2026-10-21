/*
 * A213285  Number of 6-length words w over n-ary alphabet such that for every
 *          prefix z of w we have #(z,a_i) = 0 or #(z,a_i) >= #(z,a_j) for all
 *          j>i and #(z,a_i) counts the occurrences of the i-th letter in z.
 * https://oeis.org/A213285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(-332+757*n-632*n^2+255*n^3-48*n^4+4*n^5)/4
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 42, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use signed 128‑bit arithmetic so that all sub‑expressions are
       evaluated without overflow for n up to at least 10 000. */
    __int128 N = (__int128)n;

    __int128 P = 4*N*N*N*N*N
                - 48*N*N*N*N
                + 255*N*N*N
                - 632*N*N
                + 757*N
                - 332;

    __int128 res = N * P / 4;

    return (seq_t)res;
}
