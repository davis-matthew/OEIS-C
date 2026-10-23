/*
 * A129465  Third column (m=2) sequence of triangle A129462 (v=2 member of a
 *          certain family).
 * https://oeis.org/A129465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = - n! * (n+2)! * (HarmonicNumber(n+2) - 2) for n >= 1,
 *            a(0) = 1
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 11, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Compute n! */
    seq_t fact = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        fact *= (seq_t)i;

    /* N = n! * (n+2)! = (n!)^2 * (n+1) * (n+2) */
    seq_t N = fact * fact;
    N *= (seq_t)(n + 1);
    N *= (seq_t)(n + 2);

    /* Compute S = sum_{k=1}^{n+2} N / k */
    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k <= n + 2; ++k)
    {
        seq_t term = N / (seq_t)k;  /* exact division */
        sum += term;
    }

    /* a(n) = 2*N - S */
    seq_t result = (seq_t)2 * N - sum;
    return result;
}
