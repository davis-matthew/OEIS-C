/*
 * A162261  a(n) = (2*n^3 + 5*n^2 - 7*n)/2.
 * https://oeis.org/A162261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^3 + 5*n^2 - 7*n)/2
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;       /* n^2 */
    seq_t n3 = n2 * nn;       /* n^3 */
    seq_t numerator = 2 * n3 + 5 * n2 - 7 * nn;
    return numerator / 2;
}
