/*
 * A211441  Number of ordered triples (w,x,y) with all terms in
 *          {-n,...,0,...,n} and w + x + y = 2.
 * https://oeis.org/A211441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(n^2 + n - 1) for n > 0; a(0) = 0
 * Range:     u8 n <= 8, u16 n <= 147, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    seq_t sn = (seq_t)n;
    return 3 * (sn * sn + sn - (seq_t)1);
}
