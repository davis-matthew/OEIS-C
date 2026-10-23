/*
 * A166942  One fifth of product plus sum of five consecutive nonnegative
 *          numbers.
 * https://oeis.org/A166942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^5 + 10*n^4 + 35*n^3 + 50*n^2 + 29*n + 10)/5, n >= 0
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 114, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a 128‑bit intermediate to avoid overflow for larger n. */
    unsigned __int128 N = (unsigned __int128)n;
    unsigned __int128 v = N*N*N*N*N      /* n^5          */
        + 10*N*N*N*N                     /* 10*n^4       */
        + 35*N*N*N                       /* 35*n^3       */
        + 50*N*N                         /* 50*n^2       */
        + 29*N                           /* 29*n         */
        + 10;                            /* +10          */
    v /= 5;
    return (seq_t)v;
}
