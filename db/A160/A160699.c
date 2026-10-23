/*
 * A160699  A bisection of A063522.
 * https://oeis.org/A160699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(20*n^2 - 3)
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 598, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return nn * ((seq_t)20 * nn * nn - (seq_t)3);
}
