/*
 * A046092  4 times triangular numbers: a(n) = 2*n*(n+1).
 * https://oeis.org/A046092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) = 2*n*(n+1)
 * Range:     u8 n <= 10, u16 n <= 180, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return sn * (sn + (seq_t)1) * (seq_t)2;
}
