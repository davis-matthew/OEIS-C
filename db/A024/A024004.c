/*
 * A024004  a(n) = 1 - n^6.
 * https://oeis.org/A024004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 - n^6
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 35, i64 n <= 500, i128 n <= 500,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t bn = (seq_t)n;
    seq_t n2 = bn * bn;       /* n^2 */
    seq_t n4 = n2 * n2;       /* n^4 */
    seq_t n6 = n4 * n2;       /* n^6 */
    return (seq_t)1 - n6;
}
