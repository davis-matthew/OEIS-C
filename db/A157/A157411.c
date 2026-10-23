/*
 * A157411  a(n) = 30*n^4 - 120*n^3 + 120*n^2 - 19.
 * https://oeis.org/A157411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 30*n^4 - 120*n^3 + 120*n^2 - 19
 * Range:     i8 n <= 2, i16 n <= 6, i32 n <= 92, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nm = (seq_t)n;
    seq_t n2 = nm * nm;        /* n^2 */
    seq_t n3 = nm * n2;        /* n^3 */
    seq_t n4 = n2 * n2;        /* n^4 */
    seq_t res = (seq_t)30 * n4 - (seq_t)120 * n3 + (seq_t)120 * n2 - (seq_t)19;
    return res;
}
