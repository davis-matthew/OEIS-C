/*
 * A194715  15 times triangular numbers.
 * https://oeis.org/A194715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*n*(n+1)/2
 * Range:     u8 n <= 5, u16 n <= 92, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t rn = (seq_t)n;
    seq_t tri = rn * (rn + 1) / 2;
    return tri * (seq_t)15;
}
