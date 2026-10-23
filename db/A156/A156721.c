/*
 * A156721  a(n) = 57122*n^2 - 47320*n + 9801.
 * https://oeis.org/A156721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 57122*n^2 - 47320*n + 9801
 * Range:     u8 none, u16 n <= 1, u32 n <= 274, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = n;                 /* cast index to seq_t type */
    seq_t sq = sn * sn;           /* n^2 */
    return 57122 * sq - 47320 * sn + 9801;
}
