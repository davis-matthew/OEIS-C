/*
 * A159527  Numerator of Hermite(n, 13/16).
 * https://oeis.org/A159527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0) = 1 a(1) = 13 a(n) = 13*a(n-1) - 128*(n-1)*a(n-2) for n >=
 *            2 (derived from the e.g.f. exp(13*x - 64*x^2))
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 7, i64 n <= 13, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)13;

    seq_t a0 = (seq_t)1;          /* a(n-2)  */
    seq_t a1 = (seq_t)13;         /* a(n-1)  */
    seq_t an;

    for (idx_t i = 2; i <= n; ++i) {
        /* compute 128*(i-1) as a seq_t to avoid overflow of idx_t*/
        seq_t mul = (seq_t)(128 * (i - 1)) * a0;   /* 128*(i-1)*a(n-2) */
        an = (seq_t)(13 * a1) - mul;                /* a(n)   */
        a0 = a1;
        a1 = an;
    }

    return a1;
}
