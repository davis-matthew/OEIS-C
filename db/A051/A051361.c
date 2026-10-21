/*
 * A051361  A class of Boolean functions of n variables and rank 3.
 * https://oeis.org/A051361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n-2)*(n^4+31*n^3+287*n^2+1499*n+2922)/7!
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 76, u64 n <= 1906,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 3)            /* a(0), a(1), a(2) are 0 */
        return 0;

    /* Use 128‑bit arithmetic to avoid overflow.  The formula is
       known to divide exactly by 5040 (=7!). */
    unsigned __int128 N = n;
    unsigned __int128 poly = N*N*N*N               /* n^4  */
                       + 31*N*N*N                  /* 31n^3 */
                       + 287*N*N                   /* 287n^2 */
                       + 1499*N                    /* 1499n  */
                       + 2922;                     /* constant */

    unsigned __int128 bin = N * (N - 1) * (N - 2);  /* n(n-1)(n-2) */
    unsigned __int128 num = bin * poly;            /* numerator */

    unsigned __int128 res = num / 5040;            /* divide by 7! */

    return (seq_t)res;
}
