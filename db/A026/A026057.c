/*
 * A026057  a(n) = n*(n^2 + 12*n - 25)/6.
 * https://oeis.org/A026057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n*(n^2 + 12*n - 25)/6
 * Range:     i8 n <= 3, i16 n <= 28, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;                      /* n as seq_t */
    seq_t sq = sn * sn;                       /* n^2   */
    seq_t numerator = sn * (sq + (seq_t)12 * sn - (seq_t)25);
    return numerator / (seq_t)6;              /* integer division */
}
