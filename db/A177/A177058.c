/*
 * A177058  a(n) = n^3 - 3n^2 + 3.
 * https://oeis.org/A177058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n^3 - 3*n^2 + 3
 * Range:     i8 n <= 6, i16 n <= 33, i32 n <= 1291, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t bn = (seq_t)n;          /* convert index to seq_t */
    seq_t sq = bn * bn;           /* n^2 */
    seq_t cub = sq * bn;          /* n^3 */
    seq_t three_sq = (seq_t)3 * sq;
    return cub - three_sq + (seq_t)3;
}
