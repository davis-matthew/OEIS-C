/*
 * A103832  For even n, a(n)=2n+1, for odd n, a(n)=n(n+1).
 * https://oeis.org/A103832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n+1 for even n; a(n) = n*(n+1) for odd n
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    if (n & 1)
        return sn * (sn + 1);
    else
        return (seq_t)2 * sn + 1;
}
