/*
 * A212325  Prime-generating polynomial: a(n) = n^2 + 3*n - 167.
 * https://oeis.org/A212325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n^2 + 3*n - 167
 * Range:     i8 none, i16 n <= 179, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert idx_t to seq_t before arithmetic. */
    seq_t v = (seq_t)n;
    return v * v + v * (seq_t)3 - (seq_t)167;
}
