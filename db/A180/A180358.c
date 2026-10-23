/*
 * A180358  a(n) = n^8+8n.
 * https://oeis.org/A180358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^8 + 8 n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t t2 = tn * tn;    /* n^2 */
    seq_t t4 = t2 * t2;    /* n^4 */
    seq_t t8 = t4 * t4;    /* n^8 */
    return t8 + (seq_t)8 * tn;
}
