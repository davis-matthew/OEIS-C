/*
 * A160010  Numerator of Hermite(n, 6/25).
 * https://oeis.org/A160010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 25^n * Hermite(n, 6/25) Recurrence: a(0) = 1, a(1) = 12,
 *            a(n) = 12*a(n-1) - 1250*(n-1)*a(n-2) (n ≥ 2)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)12;

    /* a_{i-2} and a_{i-1} are kept in rolling variables */
    seq_t a_prev2 = (seq_t)1;   /* a(0) */
    seq_t a_prev1 = (seq_t)12;  /* a(1) */
    seq_t a_curr;

    for (idx_t i = 2; i <= n; ++i) {
        /* coefficient = 1250*(i-1) */
        seq_t coef = (seq_t)1250 * (seq_t)(i - 1);
        a_curr = (seq_t)12 * a_prev1 - coef * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_curr;
}
