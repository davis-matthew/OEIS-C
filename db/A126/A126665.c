/*
 * A126665  a(n) = -n^2 + 9*n + 53.
 * https://oeis.org/A126665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -n^2 + 9*n + 53
 * Range:     i8 n <= 18, i16 n <= 53, i32 n <= 53, i64 n <= 53, i128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;        /* convert index to the sequence type */
    seq_t sq = nn * nn;         /* n^2 */
    seq_t val = -sq + 9 * nn + 53;
    return val;
}
