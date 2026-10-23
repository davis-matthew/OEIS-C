/*
 * A180354  a(n) = n^4 + 4*n.
 * https://oeis.org/A180354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^4 + 4*n
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* cast index to result type */
    seq_t n2 = nn * nn;           /* n^2 */
    seq_t n4 = n2 * n2;           /* n^4 */
    return n4 + (seq_t)4 * nn;    /* n^4 + 4*n */
}
