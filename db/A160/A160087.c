/*
 * A160087  Numerator of Hermite(n, 1/27).
 * https://oeis.org/A160087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 27^n * Hermite(n,1/27) Recurrence: a(0)=1, a(1)=2,
 *            a(n)=2*a(n-1) - 1458*(n-1)*a(n-2) for n>=2
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    seq_t a_prev2 = (seq_t)1;     /* a(0) */
    seq_t a_prev1 = (seq_t)2;     /* a(1) */
    seq_t a_curr = 0;

    for (idx_t i = 2; i <= n; ++i) {
        /* term = 1458 * (i-1) * a_prev2 */
        seq_t term = (seq_t)1458;
        term *= (seq_t)(i - 1);
        term *= a_prev2;

        a_curr = (seq_t)2 * a_prev1 - term;

        /* shift for next iteration */
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_curr;
}
