/*
 * A129126  Ninth diagonal of table A060850 counting partitions into parts of
 *          k kinds.
 * https://oeis.org/A129126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+6)*(n+3)*(n+1)*(4200 + 9994*n + 1571*n^2 + 74*n^3 +
 *            n^4) / 40320 This is the ninth diagonal of the table of integer
 *            partitions into parts of k kinds
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 51, u64 n <= 953, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* use 128‑bit intermediate to avoid overflow for all n tested that fit in 64 bits */
    unsigned __int128 N   = n;
    unsigned __int128 f1  = N;               /* n */
    unsigned __int128 f2  = N + 1;           /* n+1 */
    unsigned __int128 f3  = N + 3;           /* n+3 */
    unsigned __int128 f4  = N + 6;           /* n+6 */

    /* polynomial part: n^4 + 74*n^3 + 1571*n^2 + 9994*n + 4200 */
    unsigned __int128 poly =
        N * N * N * N          /* n^4 */
      + (unsigned __int128)74 * N * N * N   /* 74*n^3 */
      + (unsigned __int128)1571 * N * N     /* 1571*n^2 */
      + (unsigned __int128)9994 * N         /* 9994*n */
      + (unsigned __int128)4200;            /* constant */

    unsigned __int128 numerator   = f1 * f2 * f3 * f4 * poly;
    unsigned __int128 denominator = 40320;   /* 8! */

    unsigned __int128 result128 = numerator / denominator; /* exact division */

    return (seq_t)result128;
}
