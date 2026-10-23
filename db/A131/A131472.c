/*
 * A131472  a(n) = n^6 + n.
 * https://oeis.org/A131472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 + n
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 1625,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t t2 = tn * tn;     /* n^2 */
    seq_t t3 = t2 * tn;     /* n^3 */
    seq_t t4 = t3 * tn;     /* n^4 */
    seq_t t5 = t4 * tn;     /* n^5 */
    seq_t t6 = t5 * tn;     /* n^6 */
    return t6 + tn;         /* n^6 + n */
}
