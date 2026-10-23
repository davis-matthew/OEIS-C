/*
 * A178392  a(n) = (n^2 - 1)^2 - 1.
 * https://oeis.org/A178392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n^2 - 1)^2 - 1
 * Range:     i8 n <= 3, i16 n <= 13, i32 n <= 36, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    seq_t t = m * m - (seq_t)1;      /* t = n^2 - 1 */
    return t * t - (seq_t)1;          /* a = (n^2 - 1)^2 - 1 */
}
