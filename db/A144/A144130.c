/*
 * A144130  a(n) = ChebyshevT(4, n).
 * https://oeis.org/A144130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*n^4 - 8*n^2 + 1 (ChebyshevT(4, n))
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 152, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert index to the desired arithmetic type */
    seq_t tn = (seq_t)n;

    /* compute n^2 and n^4 */
    seq_t n2 = tn * tn;        /* n^2   */
    seq_t n4 = n2 * n2;        /* n^4   */

    /* compute the value 8*n^4 - 8*n^2 + 1 */
    seq_t result = (seq_t)8 * n4 - (seq_t)8 * n2 + (seq_t)1;
    return result;
}
