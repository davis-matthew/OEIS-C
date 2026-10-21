/*
 * A062197  Row sums of signed triangle A062139 (generalized a=2 Laguerre).
 * https://oeis.org/A062197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*n*a(n-1) - (n-1)*(n+1)*a(n-2) Initial values: a(0) = 1,
 *            a(1) = 2
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 11, i64 n <= 19, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases directly */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)2;

    /* Iterate using the recurrence; store only the last two values */
    seq_t a_nm2 = (seq_t)1;   /* a(0) */
    seq_t a_nm1 = (seq_t)2;   /* a(1) */
    seq_t a_n   = 0;          /* current value, will be overwritten */

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t term1 = (seq_t)2 * (seq_t)i * a_nm1;                 /* 2*i*a(n-1) */
        seq_t term2 = (seq_t)(i - 1) * (seq_t)(i + 1) * a_nm2;     /* (i-1)*(i+1)*a(n-2) */
        a_n = term1 - term2;                                      /* a(i) */

        /* Shift for next iteration */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
