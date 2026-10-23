/*
 * A161718  Expansion of (1+3*x^2)/(1+x)^2.
 * https://oeis.org/A161718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1; a(n) = (-1)^n * (4*n - 2) for n >= 1
 * Range:     i8 n <= 32, i16 n <= 8192, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t n_t = (seq_t)n;
    seq_t val = 4 * n_t - 2;
    if (n & 1U)   /* odd n → negative sign */
        return -val;
    else          /* even n → positive sign */
        return val;
}
