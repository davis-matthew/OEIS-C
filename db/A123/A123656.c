/*
 * A123656  a(n) = 1 + n^4 + n^6.
 * https://oeis.org/A123656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + n^4 + n^6
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t sq = sn * sn;      /* n^2 */
    seq_t n4 = sq * sq;      /* n^4 */
    seq_t n6 = sq * n4;      /* n^6 */
    return (seq_t)1 + n4 + n6;
}
