/*
 * A143938  The Wiener index of a benzenoid consisting of a linear chain of n
 *          hexagons.
 * https://oeis.org/A143938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (16*n^3 + 36*n^2 + 26*n + 3)/3
 * Range:     u8 n <= 1, u16 n <= 15, u32 n <= 644, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* cast index to seq_t first */
    seq_t n2 = nn * nn;           /* n^2   */
    seq_t n3 = n2 * nn;           /* n^3   */
    seq_t val = 16 * n3 + 36 * n2 + 26 * nn + 3;
    return val / 3;
}
