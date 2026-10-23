/*
 * A100606  a(n) = n^4 + n^3 + n.
 * https://oeis.org/A100606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 + n^3 + n
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n1 = (seq_t)n;      /* n */
    seq_t n2 = n1 * n1;       /* n^2 */
    seq_t n3 = n2 * n1;       /* n^3 */
    seq_t n4 = n3 * n1;       /* n^4 */
    return n4 + n3 + n1;
}
