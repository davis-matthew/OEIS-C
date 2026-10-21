/*
 * A062159  a(n) = n^5 - n^4 + n^3 - n^2 + n - 1.
 * https://oeis.org/A062159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = n^5 - n^4 + n^3 - n^2 + n - 1
 * Range:     i8 n <= 2, i16 n <= 8, i32 n <= 73, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n0 = (seq_t)n;        /* cast the index first */
    seq_t n2 = n0 * n0;         /* n^2 */
    seq_t n3 = n2 * n0;         /* n^3 */
    seq_t n4 = n3 * n0;         /* n^4 */
    seq_t n5 = n4 * n0;         /* n^5 */
    return n5 - n4 + n3 - n2 + n0 - 1;
}
