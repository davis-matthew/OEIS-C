/*
 * A119536  a(n) = 3*n^3 + 3*n.
 * https://oeis.org/A119536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*n*n*n + 3*n
 * Range:     u8 n <= 4, u16 n <= 27, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;          /* convert the index to seq_t */
    seq_t n2 = sn * sn;           /* n^2 */
    seq_t n3 = n2 * sn;           /* n^3 */
    seq_t res = n3 * (seq_t)3;    /* 3 * n^3 */
    res = res + sn * (seq_t)3;    /* + 3 * n */
    return res;
}
