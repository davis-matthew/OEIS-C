/*
 * A163323  The 4th Hermite polynomial evaluated at n: H_4(n) = 16*n^4 -
 *          48*n^2 + 12.
 * https://oeis.org/A163323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 16*n^4 - 48*n^2 + 12
 * Range:     i8 n <= 2, i16 n <= 6, i32 n <= 107, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* cast index to seq_t */
    seq_t n2 = nn * nn;           /* n^2 */
    seq_t n4 = n2 * n2;           /* n^4 */
    return 16 * n4 - 48 * n2 + 12;
}
