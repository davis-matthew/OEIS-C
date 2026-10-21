/*
 * A061167  a(n) = n^5 - n.
 * https://oeis.org/A061167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^5 - n
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 84, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;        /* cast index to the sequence type */
    seq_t s2 = sn * sn;         /* n^2 */
    seq_t s3 = s2 * sn;         /* n^3 */
    seq_t s4 = s3 * sn;         /* n^4 */
    seq_t s5 = s4 * sn;         /* n^5 */
    return s5 - sn;             /* n^5 - n */
}
