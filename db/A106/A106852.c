/*
 * A106852  Expansion of 1/(1-x*(1-3*x)).
 * https://oeis.org/A106852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) - 3*a(n-2); a(0) = 1, a(1) = 1
 * Range:     i8 n <= 9, i16 n <= 19, i32 n <= 39, i64 n <= 80, i128 n <= 161,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Recurrence derived from 1/(1 - x + 3x^2). */

seq_t a(idx_t n)
{
    if (n == 0ULL)
        return (seq_t)1;
    if (n == 1ULL)
        return (seq_t)1;

    seq_t a_nm2 = (seq_t)1; /* a[0] */
    seq_t a_nm1 = (seq_t)1; /* a[1] */

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t a_n = a_nm1 - 3 * a_nm2;   /* compute a[i] */
        a_nm2 = a_nm1;   /* shift one step back */
        a_nm1 = a_n;
    }

    return a_nm1;
}
