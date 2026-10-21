/*
 * A212135  Number of (w,x,y,z) with all terms in {1,...,n} and median<mean.
 * https://oeis.org/A212135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n - 1)*(n^2 - n + 2)/2
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 272, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    seq_t sn = (seq_t)n;
    seq_t n1 = sn - (seq_t)1;
    seq_t t  = sn * sn - sn + (seq_t)2;   /* n^2 - n + 2 */
    seq_t prod = sn * n1;
    prod = prod * t;
    return prod / (seq_t)2;
}
