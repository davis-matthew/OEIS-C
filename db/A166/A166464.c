/*
 * A166464  a(n) = (3 + 2*n + 6*n^2 + 4*n^3)/3.
 * https://oeis.org/A166464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3 + 2*n + 6*n^2 + 4*n^3)/3
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    seq_t numerator = 4 * n3 + 6 * n2 + 2 * nn + 3;
    return numerator / 3;
}
