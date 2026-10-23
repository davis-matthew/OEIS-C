/*
 * A159542  Numerator of Hermite(n, 14/17).
 * https://oeis.org/A159542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0)=1, a(1)=28, a(n+1)=28*a(n)-578*n*a(n-1) (for n≥1)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 5, i64 n <= 11, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    if (n == 1) {
        return (seq_t)28;
    }

    seq_t a_prev2 = (seq_t)1;      // a(0)
    seq_t a_prev1 = (seq_t)28;     // a(1)

    for (idx_t i = 2; i <= n; ++i) {
        /* term = 578*(i-1)*a_prev2 uses seq_t arithmetic to avoid overflow */
        seq_t term = (seq_t)578 * (seq_t)(i - 1) * a_prev2;
        seq_t a_curr = (seq_t)28 * a_prev1 - term;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_prev1;
}
