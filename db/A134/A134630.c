/*
 * A134630  a(n) = 5*n^5 - 3*n^3 - 2*n^2.
 * https://oeis.org/A134630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n^5 - 3*n^3 - 2*n^2
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 61, u64 n <= 1000,
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
    seq_t n5 = n3 * n2;          /* n^5 */
    seq_t res = 5 * n5 - 3 * n3 - 2 * n2;
    return res;
}
