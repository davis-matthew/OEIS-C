/*
 * A123659  a(n) = 1 + n^4 + n^6 + n^9 + n^10 + n^14.
 * https://oeis.org/A123659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + n^4 + n^6 + n^9 + n^10 + n^14
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 23, u128 n <= 565,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;           /* t = n */
    seq_t n2 = tn * tn;            /* n^2 */
    seq_t n3 = n2 * tn;            /* n^3 */
    seq_t n4 = n2 * n2;            /* n^4 */
    seq_t n6 = n4 * n2;            /* n^6 */
    seq_t n9 = n6 * n3;            /* n^9 */
    seq_t n10 = n9 * tn;           /* n^10 */
    seq_t n14 = n10 * n4;          /* n^14 */

    return (seq_t)1 + n4 + n6 + n9 + n10 + n14;
}
