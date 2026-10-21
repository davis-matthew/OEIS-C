/*
 * A083727  a(n) = n * (2^n - 8).
 * https://oeis.org/A083727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n*(2^n - 8)
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 26, i64 n <= 31, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* compute 2^n in seq_t by repeated multiplication */
    seq_t pow = 1;
    for (idx_t i = 0; i < n; ++i)
        pow *= 2;

    /* a(n) = n * (pow - 8) */
    seq_t term = pow - 8;
    return (seq_t)n * term;
}
