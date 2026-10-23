/*
 * A172226  Number of ways to place 3 nonattacking wazirs on an n X n board.
 * https://oeis.org/A172226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-2)*(n^5 + 2*n^4 - 11*n^3 - 10*n^2 + 42*n - 12)/6 ,
 *            n>=2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 53, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* a(1) = 0 */
    if (n < 2)
        return 0;

    seq_t N   = n;
    seq_t N2  = N * N;
    seq_t N3  = N2 * N;
    seq_t N4  = N3 * N;
    seq_t N5  = N4 * N;

    seq_t poly = N5
                + (seq_t)2 * N4
                - (seq_t)11 * N3
                - (seq_t)10 * N2
                + (seq_t)42 * N
                - (seq_t)12;

    seq_t result = (N - (seq_t)2) * poly;
    result = result / (seq_t)6;   /* exact division */

    return result;
}
