/*
 * A087887  a(n) = 18*n^3 + 6*n^2.
 * https://oeis.org/A087887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 18*n^3 + 6*n^2
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* convert index to seq_t */
    seq_t n2 = nn * nn;           /* n^2 */
    seq_t n3 = n2 * nn;           /* n^3 */
    return 18 * n3 + 6 * n2;      /* 18*n^3 + 6*n^2 */
}
