/*
 * A232568  Number of non-equivalent binary n X n matrices with three pairwise
 *          nonadjacent 1's.
 * https://oeis.org/A232568
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^6 - 15*n^4 + 20*n^3 + 50*n^2 - 116*n + 48)/48 if n is
 *            even, a(n) = (n^6 - 15*n^4 + 28*n^3 + 29*n^2 - 76*n - 15)/48 if
 *            n is odd
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 76, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;                   /* values below offset are unused */

    /* Use a wider type for intermediate calculations to avoid overflow
       for 64‑bit seq_t.  __uint128_t is available on common compilers. */
    __uint128_t N  = n;
    __uint128_t n2 = N * N;
    __uint128_t n3 = n2 * N;
    __uint128_t n4 = n2 * n2;
    __uint128_t n6 = n3 * n3;

    __uint128_t num;
    if ((n & 1) == 0) {                    /* n is even */
        num = n6
            - (__uint128_t)15 * n4
            + (__uint128_t)20 * n3
            + (__uint128_t)50 * n2
            - (__uint128_t)116 * N
            + (__uint128_t)48;
    } else {                               /* n is odd */
        num = n6
            - (__uint128_t)15 * n4
            + (__uint128_t)28 * n3
            + (__uint128_t)29 * n2
            - (__uint128_t)76 * N
            - (__uint128_t)15;
    }

    return (seq_t)(num / (__uint128_t)48);
}
