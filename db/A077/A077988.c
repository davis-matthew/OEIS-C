/*
 * A077988  Expansion of 1/(1+2*x-2*x^3).
 * https://oeis.org/A077988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -2*a(n-1) + 2*a(n-3) with a(0)=1, a(1)=-2, a(2)=4
 * Range:     i8 n <= 10, i16 n <= 23, i32 n <= 48, i64 n <= 101,
 *            i128 n <= 202, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the first three terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)(-2);
    if (n == 2) return (seq_t)4;

    /* Use a rolling window of the three most recent coefficients */
    seq_t a_prev3 = (seq_t)1;   /* a[0] */
    seq_t a_prev2 = (seq_t)(-2);/* a[1] */
    seq_t a_prev1 = (seq_t)4;   /* a[2] */
    seq_t ai;

    for (idx_t i = 3; i <= n; ++i) {
        ai = (-2) * a_prev1 + (seq_t)2 * a_prev3;
        a_prev3 = a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = ai;
    }
    return a_prev1;
}
