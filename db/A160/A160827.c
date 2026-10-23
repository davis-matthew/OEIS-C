/*
 * A160827  a(n) = 3*n^4 + 12*n^3 + 30*n^2 + 36*n + 17.
 * https://oeis.org/A160827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n^4 + 12*n^3 + 30*n^2 + 36*n + 17
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 193, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t sq = sn * sn;          /* n^2 */
    seq_t n3 = sn * sq;          /* n^3 */
    seq_t n4 = sq * sq;          /* n^4 */
    seq_t res = n4 * 3 + n3 * 12 + sq * 30 + sn * 36 + 17;
    return res;
}
