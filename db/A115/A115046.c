/*
 * A115046  G.f.: (1+x^2)^2*(x^4-6*x^3+1)/(x^2-1)^4.
 * https://oeis.org/A115046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -n/12 + 3/4 - n^3/6 + 3n^2/4 * + (-1)^n*(-3n^2/4 + 29n/12
 *            - 3/4 + n^3/3), n>0. * a(0) = 1
 * Range:     i8 n <= 3, i16 n <= 18, i32 n <= 46, i64 n <= 46, i128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Work in seq_t to keep exact arithmetic. */
    seq_t N  = (seq_t)n;
    seq_t n2 = N * N;
    seq_t n3 = n2 * N;

    /* Build the expression inside the (-1)^n factor. */
    seq_t expr = -9 * n2 + 29 * N - 9 + 4 * n3;

    /* Numerator of the final formula multiplied by 12. */
    seq_t denom12 = 12;
    seq_t numerator = -N + 9 - 2 * n3 + 9 * n2
                      + ((n & 1) == 0 ? expr : -expr);

    /* Divide by 12 to obtain the integer result. */
    return numerator / denom12;
}
