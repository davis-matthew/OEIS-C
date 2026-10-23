/*
 * A144298  Number of cycles of length 3 in the queen graph associated with an
 *          n X n chessboard.
 * https://oeis.org/A144298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4 + n^3 - 5*n^2 + ((n&1)?3:2)*n) / 2, n ≥ 0
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 277, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast the index to the computation type */
    seq_t N = (seq_t)n;

    /* compute powers of n */
    seq_t n2 = N * N;          /* n^2 */
    seq_t n3 = n2 * N;         /* n^3 */
    seq_t n4 = n3 * N;         /* n^4 */

    /* coefficient for the linear term */
    seq_t coeff = ((n & (idx_t)1) ? (seq_t)3 : (seq_t)2);

    /* numerator of the rational expression */
    seq_t num = n4               /* n^4 */
              + n3              /* + n^3 */
              - 5 * n2          /* - 5*n^2 */
              + coeff * N;      /* + coeff*n */

    /* final integer division by 2 */
    return num / 2;
}
