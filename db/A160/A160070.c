/*
 * A160070  Numerator of Hermite(n, 3/26).
 * https://oeis.org/A160070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 3*a(n-1) - 338*(n-1)*a(n-2) with a(0) = 1, a(1) = 3
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms            */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    /* a_{n-2} and a_{n-1} keep the previous two values          */
    seq_t a_prev2 = (seq_t)1; /* a(0) */
    seq_t a_prev1 = (seq_t)3; /* a(1) */

    /* Iterate from 2 up to n, inclusive          */
    for (idx_t i = 2; i <= n; ++i) {
        /* a(i) = 3*a(i-1) - 338*(i-1)*a(i-2)     */
        seq_t mult = (seq_t)338 * (seq_t)(i - 1) * a_prev2;
        seq_t cur  = (seq_t)3 * a_prev1 - mult;

        /* Shift the window: a_prev2 <- a_prev1, a_prev1 <- cur   */
        a_prev2 = a_prev1;
        a_prev1 = cur;
    }

    return a_prev1;
}
