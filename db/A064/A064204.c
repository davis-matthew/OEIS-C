/*
 * A064204  a(n) = 280*binomial(n+4,9) + 280*binomial(n+4,8) +
 *          105*binomial(n+3,7) + 77*binomial(n+3,6) + 43*binomial(n+2,5) -
 *          16*binomial(n+2,4) + 20*binomial(n+1,3) - floor(n*(n^2 - 1)*(n^2 -
 *          4)*(n-3)/360).
 * https://oeis.org/A064204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n*(-720 + 1080*n + 404*n^2 - 1098*n^3 + 282*n^4 + 9*n^5
 *            + 33*n^6 + 9*n^7 + n^8))/1296
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 25, u64 n <= 305, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t and related types */

seq_t a(idx_t n)
{
    /* Use signed 128‑bit arithmetic for intermediate precision.  */
    __int128 N = n;

    /* Compute the polynomial part.  */
    __int128 poly = -720
                    + 1080*N
                    + 404*N*N
                    - 1098*N*N*N
                    + 282*N*N*N*N
                    + 9*N*N*N*N*N
                    + 33*N*N*N*N*N*N
                    + 9*N*N*N*N*N*N*N
                    + N*N*N*N*N*N*N*N;

    /* Numerator: n * poly.  */
    __int128 num = N * poly;

    /* Final result.  The division is exact.  */
    return (seq_t)(num / 1296);
}
