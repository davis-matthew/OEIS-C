/*
 * A057721  a(n) = n^4 + 3*n^2 + 1.
 * https://oeis.org/A057721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 + 3*n^2 + 1
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t sq = sn * sn;           /* n^2 */
    seq_t fourth = sq * sq;       /* n^4 */
    return fourth + (seq_t)3 * sq + (seq_t)1;
}
