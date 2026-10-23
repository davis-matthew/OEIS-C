/*
 * A156798  a(n) = n^4 + 5*n^2 + 4.
 * https://oeis.org/A156798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 + 5*n^2 + 4
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;          /* cast index to seq_t */
    seq_t sq = sn * sn;           /* n^2 */
    seq_t n4 = sq * sq;           /* n^4 */
    return n4 + (5 * sq) + (seq_t)4;
}
