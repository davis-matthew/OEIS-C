/*
 * A143945  Wiener index of the grid P_n x P_n, where P_n is the path graph on
 *          n vertices.
 * https://oeis.org/A143945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3*(n^2-1)/3
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 91, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;               /* n as seq_t */
    seq_t n2 = nn * nn;                /* n^2   */
    seq_t n3 = n2 * nn;                /* n^3   */
    seq_t temp = n3 * (n2 - (seq_t)1); /* n^3*(n^2-1) */
    return temp / (seq_t)3;            /* divide by 3 */
}
