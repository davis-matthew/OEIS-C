/*
 * A134479  Row sums of triangle A134478.
 * https://oeis.org/A134479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(0) = 1; for n > 0: a(n) = 3 * n * (n + 1) / 2
 * Range:     u8 n <= 12, u16 n <= 208, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;
    seq_t sn = (seq_t)n;
    return (seq_t)((seq_t)3 * sn * (sn + (seq_t)1) / (seq_t)2);
}
