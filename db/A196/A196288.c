/*
 * A196288  a(n) = n^8 + n.
 * https://oeis.org/A196288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = n^8 + n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 255,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;          /* cast index to sequence type */
    seq_t tn2 = tn * tn;          /* n^2 */
    seq_t tn4 = tn2 * tn2;        /* n^4 */
    seq_t tn8 = tn4 * tn4;        /* n^8 */
    return tn8 + tn;              /* n^8 + n */
}
