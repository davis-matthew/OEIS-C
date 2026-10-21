/*
 * A203425  a(n) = w(n+1)/(4*w(n)), where w = A203424.
 * https://oeis.org/A203425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (1/4)*(-2*(n+1))^n
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 13, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base = -2*(n+1) (in seq_t) */
    seq_t base = -(seq_t)2 * ((seq_t)n + 1);
    seq_t result = 1;
    for (idx_t i = 0; i < n; ++i)
        result = result * base;
    return result / 4;
}
