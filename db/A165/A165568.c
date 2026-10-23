/*
 * A165568  a(n) = -1 - 2*n + n^2 + 2*n^3 + n^4.
 * https://oeis.org/A165568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -1 - 2*n + n^2 + 2*n^3 + n^4
 * Range:     i8 n <= 2, i16 n <= 12, i32 n <= 214, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;          /* n^2 */
    seq_t n3 = n2 * sn;          /* n^3 */
    seq_t n4 = n3 * sn;          /* n^4 */
    seq_t result = -1 - 2 * sn + n2 + 2 * n3 + n4;
    return result;
}
