/*
 * A176414  Expansion of (7+8*x)/(1+2*x).
 * https://oeis.org/A176414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*(-2)^n for n > 0, a(0) = 7
 * Range:     i8 n <= 5, i16 n <= 13, i32 n <= 29, i64 n <= 61, i128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 7;

    seq_t result = 3;          /* 3*(-2)^0 */
    for (idx_t i = 0; i < n; ++i)
        result = -2 * result;  /* multiply by (-2) n times */

    return result;
}
