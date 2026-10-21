/*
 * A212982  Number of (w,x,y) with all terms in {0,...,n} and w<x+y and x<=y.
 * https://oeis.org/A212982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (20*n^3+66*n^2+52*n-3*(-1)^n+3)/48 * This simplifies to
 *            a(n) = (20*n^3+66*n^2+52*n)/48 for even n, * and a(n) =
 *            (20*n^3+66*n^2+52*n+6)/48 for odd n
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 634, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nt  = (seq_t)n;          /* convert n to seq_t for arithmetic   */
    seq_t n2  = nt * nt;           /* n^2                          */
    seq_t n3  = n2 * nt;           /* n^3                          */
    seq_t num = 20 * n3 + 66 * n2 + 52 * nt; /* base numerator              */
    if (n & 1)                      /* add 6 if n is odd            */
        num += 6;
    return num / 48;                 /* integer division, exact       */
}
