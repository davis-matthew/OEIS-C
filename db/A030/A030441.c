/*
 * A030441  Values of Newton-Gregory forward interpolating polynomial
 *          (1/3)*(2*n - 3)*(2*n^2 - 3*n + 4).
 * https://oeis.org/A030441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (1/3)*(2*n - 3)*(2*n^2 - 3*n + 4)
 * Range:     i8 n <= 5, i16 n <= 30, i32 n <= 41, i64 n <= 41, i128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n0 = (seq_t)n;
    seq_t t1 = 2*n0 - (seq_t)3;
    seq_t t2 = 2*n0*n0 - (seq_t)3*n0 + (seq_t)4;
    return (t1 * t2) / (seq_t)3;
}
