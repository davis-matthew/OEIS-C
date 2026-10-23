/*
 * A159904  Numerator of Hermite(n, 17/23).
 * https://oeis.org/A159904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0)=1, a(1)=34; a(n)=34*a(n-1)-2*(n-1)*529*a(n-2) for n>=2
 * Range:     i8 n <= 2, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)34;

    seq_t a_prev2 = (seq_t)1;  /* a(0) */
    seq_t a_prev1 = (seq_t)34; /* a(1) */
    seq_t a_curr  = a_prev1;

    for (idx_t i = 1; i < n; ++i)   /* compute up to a(n) */
    {
        /* a_next = 34*a_prev1 - 2*i*529*a_prev2 */
        seq_t term = (seq_t)34 * a_prev1;
        seq_t mul  = (seq_t)2 * (seq_t)i * (seq_t)529 * a_prev2;
        a_curr = term - mul;

        /* shift for next iteration */
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_curr;
}
