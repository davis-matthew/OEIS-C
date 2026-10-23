/*
 * A131479  a(n) = floor(n^4/4).
 * https://oeis.org/A131479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n^4 / 4)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 268, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;           /* n^2 */
    seq_t n4 = n2 * n2;           /* n^4 */
    return n4 / (seq_t)4;         /* floor division */
}
