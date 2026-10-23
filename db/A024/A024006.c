/*
 * A024006  a(n) = 1 - n^8.
 * https://oeis.org/A024006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 - n^8
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 14, i64 n <= 234, i128 n <= 420,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t r = tn * tn;    /* n^2 */
    r = r * r;            /* n^4 */
    r = r * r;            /* n^8 */
    return ((seq_t)1) - r;
}
