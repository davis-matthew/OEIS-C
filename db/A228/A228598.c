/*
 * A228598  The Wiener index of the graph obtained by applying Mycielski's
 *          construction to the crown graph G(n) (n>=3).
 * https://oeis.org/A228598
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*n^2 + 8*n
 * Range:     u8 n <= 4, u16 n <= 70, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return nn * (nn * 13 + 8);
}
