/*
 * A191237  Expansion of e.g.f. exp(x+x^3+x^5).
 * https://oeis.org/A191237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*(n-1)*(n-2)*a(n-3) +
 *            5*(n-1)*(n-2)*(n-3)*(n-4)*a(n-5), a(0) = 1 (derived from the
 *            exponential generating function exp(x + x³ + x⁵))
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* A sliding window of the five most recent terms.  The indices below
       represent a(i-k) where i is the current iteration number.  The
       window is updated at the end of each loop iteration.        */
    seq_t last[5] = { (seq_t)1, 0, 0, 0, 0 /* a(0), a(-1), a(-2), a(-3), a(-4) */ };

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t res = last[0];                /* a(i-1) */

        if (i >= 3) {                       /* term for a(i-3)   */
            seq_t term = last[2];
            term *= (seq_t)(i - 1);
            term *= (seq_t)(i - 2);
            term *= (seq_t)3;
            res += term;
        }

        if (i >= 5) {                       /* term for a(i-5)   */
            seq_t term = last[4];
            term *= (seq_t)(i - 1);
            term *= (seq_t)(i - 2);
            term *= (seq_t)(i - 3);
            term *= (seq_t)(i - 4);
            term *= (seq_t)5;
            res += term;
        }

        /* Shift the window:  a(i-1) becomes a(i-2), …, a(i-4) becomes a(i-5) */
        last[4] = last[3];
        last[3] = last[2];
        last[2] = last[1];
        last[1] = last[0];
        last[0] = res;                      /* a(i) */
    }

    return last[0];                         /* a(n) */
}
