/*
 * A024183  Second elementary symmetric function of 3,4,...,n+3.
 * https://oeis.org/A024183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(3*n^2 + 35*n + 106)/24
 * Range:     u8 none, u16 n <= 9, u32 n <= 426, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast the index to the sequence type to maintain type safety */
    seq_t sn = (seq_t)n;
    seq_t a = sn * (sn + 1);                       /* n*(n+1)       */
    seq_t b = 3 * sn * sn + 35 * sn + 106;         /* 3*n^2 + 35*n + 106 */
    a = a * b;                                     /* n*(n+1)*(3*n^2+35*n+106) */
    return a / (seq_t)24;                          /* divide by 24 */
}
