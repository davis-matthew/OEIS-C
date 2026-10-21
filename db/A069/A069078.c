/*
 * A069078  a(n) = n*(4*n^4 + 1).
 * https://oeis.org/A069078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(4*n^4 + 1)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;       /* promote index to seq_t */
    seq_t n2 = sn * sn;        /* n^2 */
    seq_t n4 = n2 * n2;        /* n^4 */
    seq_t t  = 4 * n4 + 1;     /* 4*n^4 + 1 */
    return sn * t;             /* n * (4*n^4 + 1) */
}
