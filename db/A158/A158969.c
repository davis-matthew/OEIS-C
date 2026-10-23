/*
 * A158969  Numerator of Hermite(n, 5/6).
 * https://oeis.org/A158969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 5*a(n-1) - 18*(n-1)*a(n-2), with a(0) = 1, a(1) = 5
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 10, i64 n <= 18, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)5;   /* a(1) */
    seq_t an = 0;          /* placeholder for a(i) */

    for (idx_t i = 2; i <= n; ++i)
    {
        an = (seq_t)5 * a1
             - ( ((seq_t)18) * ((seq_t)(i - 1)) * a0 );
        a0 = a1;
        a1 = an;
    }

    return an;
}
