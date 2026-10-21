/*
 * A083196  a(n) = 8*n^4 + 9*n^2 + 2.
 * https://oeis.org/A083196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*n^4 + 9*n^2 + 2
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* convert index to seq_t */
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n4 = n2 * n2;          /* n^4 */
    return (seq_t)8 * n4 + (seq_t)9 * n2 + (seq_t)2;
}
