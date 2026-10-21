/*
 * A077874  Expansion of (1-x)^(-1)/(1-x+2*x^2-2*x^3).
 * https://oeis.org/A077874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) satisfies the linear recurrence a(n) = 2*a(n-1) - 3*a(n-2)
 *            + 4*a(n-3) - 2*a(n-4) with initial values a(0)=1, a(1)=2,
 *            a(2)=1, a(3)=0
 * Range:     i8 n <= 16, i16 n <= 32, i32 n <= 46, i64 n <= 46, i128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial values */
    static const seq_t init[4] = { (seq_t)1, (seq_t)2, (seq_t)1, (seq_t)0 };

    /* handle the first four indices directly */
    if (n < 4)
        return init[n];

    /* variables for the last four computed terms */
    seq_t a0 = init[0]; /* a(n-4) */
    seq_t a1 = init[1]; /* a(n-3) */
    seq_t a2 = init[2]; /* a(n-2) */
    seq_t a3 = init[3]; /* a(n-1) */
    seq_t an;           /* a(n) to be computed */

    idx_t i;
    for (i = 4; i <= n; ++i) {
        an = 2 * a3 - 3 * a2 + 4 * a1 - 2 * a0;
        a0 = a1;  /* shift window: a(n-4) becomes a(n-3) */
        a1 = a2;  /* a(n-3) becomes a(n-2) */
        a2 = a3;  /* a(n-2) becomes a(n-1) */
        a3 = an;  /* a(n-1) becomes a(n)   */
    }
    return a3;   /* a(n) is now stored in a3 */
}
