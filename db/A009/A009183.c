/*
 * A009183  Expansion of e.g.f.: cosh(x)/cosh(log(1+x)).
 * https://oeis.org/A009183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1, a(1)=0. For n ≥ 2, let D = n*(n-1)/2. If n is even: a(n)
 *            = 1 – n*a(n−1) – D*a(n−2) If n is odd: a(n) = n – n*a(n−1) –
 *            D*a(n−2)
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 14, i64 n <= 22, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    seq_t a_nm2 = (seq_t)1;   /* a(0) */
    seq_t a_nm1 = (seq_t)0;   /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t term   = ((i & 1) == 0) ? (seq_t)1 : (seq_t)i;
        seq_t ni     = (seq_t)i;
        seq_t mult1  = ni * a_nm1;                         /* n*a(n−1) */
        seq_t factor = (ni * ((seq_t)i - 1)) / 2;          /* D = n*(n−1)/2 */
        seq_t mult2  = factor * a_nm2;                    /* D*a(n−2) */
        seq_t a_i    = term - mult1 - mult2;
        a_nm2 = a_nm1;
        a_nm1 = a_i;
    }
    return a_nm1;
}
