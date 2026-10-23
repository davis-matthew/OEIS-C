/*
 * A036404  a(n) = ceiling(n^2/5).
 * https://oeis.org/A036404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n^2/5)
 * Range:     u8 n <= 15, u16 n <= 571, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sq = (seq_t)n * ((seq_t)n);
    return (sq + 4) / 5;
}
