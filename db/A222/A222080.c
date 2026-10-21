/*
 * A222080  1 = Sum_{n>=0} a(n) * x^n * (1 - (2*n+1)*x)^2.
 * https://oeis.org/A222080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(2*n-1)*a(n-1) - (2*n-3)^2*a(n-2) initial values: a(0)
 *            = 1, a(1) = 2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special cases for the first two terms */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* Use only two previous values – this is O(n) and uses constant space */
    seq_t a_prev2 = (seq_t)1;   /* a(0) */
    seq_t a_prev1 = (seq_t)2;   /* a(1) */
    seq_t a_curr  = (seq_t)0;

    for (idx_t i = 2; i <= n; ++i) {
        /* term1 = 2 * (2*i - 1) * a_prev1 */
        seq_t term1 = (seq_t)2 * (seq_t)(2 * i - 1) * a_prev1;

        /* term2 = (2*i - 3)^2 * a_prev2 */
        seq_t t = (seq_t)(2 * i - 3);
        t = t * t;
        seq_t term2 = t * a_prev2;

        a_curr = term1 - term2;   /* recurrence */

        /* shift for next iteration */
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_curr;
}
