/*
 * A228935  a(n) = (3 - 6*n)*(-1)^n.
 * https://oeis.org/A228935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, cofr, easy
 * Formula:   a(n) = (3 - 6*n)*(-1)^n (n≥1), a(0) = 0
 * Range:     i8 n <= 21, i16 n <= 49, i32 n <= 49, i64 n <= 49, i128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset term explicitly */
    if (n == 0) return 0;

    /* Compute sign = (-1)^n.  For even n it is 1, for odd n it is -1. */
    seq_t sign = (n & 1ULL) ? -1 : 1;

    /* Compute 3 - 6*n as a seq_t, then multiply by the sign. */
    seq_t term = (seq_t)(3 - (seq_t)6 * (seq_t)n) * sign;
    return term;
}
