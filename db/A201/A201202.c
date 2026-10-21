/*
 * A201202  Row sums of triangle A201201: first associated monic Laguerre
 *          polynomials with parameter alpha=1 evaluated at x=1.
 * https://oeis.org/A201202
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -((2*n+1)*a(n-1) + n*(n+1)*a(n-2)), with initial values
 *            a(0) = 1, a(1) = -3
 * Range:     i8 n <= 5, i16 n <= 6, i32 n <= 11, i64 n <= 19, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-3;

    seq_t a_nm2 = (seq_t)1;   /* a(0) */
    seq_t a_nm1 = (seq_t)-3;  /* a(1) */

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t term1 = ((seq_t)i * (seq_t)2 + (seq_t)1) * a_nm1;
        seq_t term2 = (seq_t)i * ((seq_t)i + (seq_t)1) * a_nm2;
        seq_t ai = -(term1 + term2);
        a_nm2 = a_nm1;
        a_nm1 = ai;
    }

    return a_nm1;
}
