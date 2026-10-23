/*
 * A022273  a(n) = n*(15*n + 1)/2.
 * https://oeis.org/A022273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(15*n + 1)/2
 * Range:     u8 n <= 5, u16 n <= 93, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return nn * (nn * 15 + 1) / 2;
}
