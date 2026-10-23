/*
 * A160842  Number of lines through at least 2 points of a 2 X n grid of
 *          points.
 * https://oeis.org/A160842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = { 0, 1, n^2 + 2 (for n > 1) }
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    seq_t nn = (seq_t)n;
    return nn * nn + (seq_t)2;
}
