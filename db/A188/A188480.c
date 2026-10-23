/*
 * A188480  a(n) = (n^4 + 16*n^3 + 65*n^2 + 26*n + 12)/12.
 * https://oeis.org/A188480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4 + 16*n^3 + 65*n^2 + 26*n + 12)/12
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 340, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;        /* n^2 */
    seq_t n3 = n2 * nn;        /* n^3 */
    seq_t n4 = n3 * nn;        /* n^4 */
    seq_t num = n4 + 16 * n3 + 65 * n2 + 26 * nn + 12;
    return num / 12;
}
