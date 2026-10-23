/*
 * A159622  Numerator of Hermite(n, 5/19).
 * https://oeis.org/A159622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 10*a(n-1) - 722*(n-1)*a(n-2) (n ≥ 2) Initial values: a(0)
 *            = 1, a(1) = 10
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 5, i64 n <= 10, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;

    seq_t a_prev2 = (seq_t)1;   /* a(0) */
    seq_t a_prev1 = (seq_t)10;  /* a(1) */
    seq_t a_curr = 0;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* mult = 722 * (i-1) */
        seq_t mult = (seq_t)722 * (seq_t)(i - 1);
        /* a_curr = 10*a_prev1 - mult * a_prev2 */
        a_curr = (seq_t)10 * a_prev1 - mult * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_curr;
}
