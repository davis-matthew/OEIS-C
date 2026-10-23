/*
 * A015268  Gaussian binomial coefficient [ n,3 ] for q = -3.
 * https://oeis.org/A015268
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) satisfies the linear recurrence a(n) = -20 a(n-1) + 210
 *            a(n-2) + 540 a(n-3) - 729 a(n-4), for all n ≥ 7. Initial values:
 *            a(3) = 1 a(4) = -20 a(5) = 610 a(6) = -15860 (These values are
 *            the first terms of the sequence.)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 9, i64 n <= 16, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 3) return (seq_t)1;
    if (n == 4) return (seq_t)-20;
    if (n == 5) return (seq_t)610;
    if (n == 6) return (seq_t)-15860;

    /* Recurrence variables: a_{k-4}, a_{k-3}, a_{k-2}, a_{k-1} */
    seq_t a_nm4 = (seq_t)1;          /* a(3) */
    seq_t a_nm3 = (seq_t)-20;        /* a(4) */
    seq_t a_nm2 = (seq_t)610;        /* a(5) */
    seq_t a_nm1 = (seq_t)-15860;     /* a(6) */
    seq_t a_n   = 0;                 /* placeholder */

    for (idx_t k = 7; k <= n; ++k)
    {
        a_n = (-20) * a_nm1 + (210) * a_nm2 + (540) * a_nm3 - (729) * a_nm4;

        /* shift variables forward for next iteration */
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
