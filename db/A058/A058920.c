/*
 * A058920  a(n) = 2*n^4 + 2*n^3 + 3*n^2 + 2*n + 1.
 * https://oeis.org/A058920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^4 + 2*n^3 + 3*n^2 + 2*n + 1
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 215, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t  = (seq_t)n;          /* cast index to sequence type */
    seq_t n2 = t * t;             /* n^2 */
    seq_t n3 = n2 * t;            /* n^3 */
    seq_t n4 = n2 * n2;           /* n^4 */

    seq_t res = (seq_t)2 * n4 + (seq_t)2 * n3
              + (seq_t)3 * n2 + (seq_t)2 * t
              + (seq_t)1;
    return res;
}
