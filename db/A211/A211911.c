/*
 * A211911  Number of lower triangular n X n arrays colored with integers 0
 *          upwards introduced in row major order, with no element equal to
 *          any horizontal, vertical, diagonal or antidiagonal neighbor, and
 *          containing the value n(n+1)/2-2.
 * https://oeis.org/A211911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 + 2*n^3 - 17*n^2 + 22*n)/8 for n > 1, a(1) = 0
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return 0;
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    seq_t n4 = n2 * n2;          /* n^4 */
    seq_t num = n4 + (seq_t)2 * n3 - (seq_t)17 * n2 + (seq_t)22 * nn;
    return num / (seq_t)8;
}
