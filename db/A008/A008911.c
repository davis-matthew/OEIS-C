/*
 * A008911  a(n) = n^2*(n^2 - 1)/6.
 * https://oeis.org/A008911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(n^2 - 1)/6
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 399, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;
    seq_t sq = (seq_t)n * (seq_t)n;
    seq_t num = sq * (sq - 1);
    return num / 6;
}
