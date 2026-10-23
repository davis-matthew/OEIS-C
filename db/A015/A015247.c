/*
 * A015247  a(n) = (2*n - 15)*n^2.
 * https://oeis.org/A015247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (2*n - 15)*n^2
 * Range:     i8 n <= 8, i16 n <= 28, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_t = (seq_t)n;
    seq_t tmp = 2 * n_t - 15;   /* 2*n - 15 */
    seq_t res = tmp * n_t * n_t;/* (2*n - 15)*n^2 */
    return res;
}
