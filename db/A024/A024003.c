/*
 * A024003  a(n) = 1 - n^5.
 * https://oeis.org/A024003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 - n^5
 * Range:     i8 n <= 2, i16 n <= 8, i32 n <= 73, i64 n <= 555, i128 n <= 555,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;      /* convert index to seq_t */
    seq_t n2 = N * N;        /* n^2 */
    seq_t n3 = n2 * N;       /* n^3 */
    seq_t n4 = n3 * N;       /* n^4 */
    seq_t n5 = n4 * N;       /* n^5 */
    return 1 - n5;           /* a(n) = 1 - n^5 */
}
