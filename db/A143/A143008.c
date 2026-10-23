/*
 * A143008  Crystal ball sequence for the A2 x A2 lattice.
 * https://oeis.org/A143008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (3*n^4 + 6*n^3 + 9*n^2 + 6*n + 2) / 2
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;          /* convert index to seq_t */
    seq_t n2 = sn * sn;           /* n^2 */
    seq_t n3 = n2 * sn;           /* n^3 */
    seq_t n4 = n3 * sn;           /* n^4 */
    seq_t num = 3 * n4 + 6 * n3 + 9 * n2 + 6 * sn + 2;
    return num / 2;
}
