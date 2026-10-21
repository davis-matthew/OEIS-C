/*
 * A071270  a(n) = n^2*(2*n^2 + 1)/3.
 * https://oeis.org/A071270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 * (2*n^2 + 1) / 3
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 282, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nt = (seq_t)n;
    seq_t n2 = nt * nt;                 /* n^2                                   */
    seq_t num = n2 * (((seq_t)2 * n2) + 1);   /* n^2*(2*n^2 + 1)                       */
    return num / 3;                     /* exact division by 3                    */
}
