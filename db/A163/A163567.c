/*
 * A163567  Number of reduced words of length n in Coxeter group on 33
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^5 = I.
 * https://oeis.org/A163567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 31*a(n-1)+31*a(n-2)+31*a(n-3)+31*a(n-4)-496*a(n-5) (for n
 *            >= 6) with initial values a(0) = 1, a(1) = 33, a(2) = 1056, a(3)
 *            = 33792, a(4) = 1081344, a(5) = 34602480
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The first six terms are hard‑coded */
    static const seq_t base[6] = {
        (seq_t)1,
        (seq_t)33,
        (seq_t)1056,
        (seq_t)33792,
        (seq_t)1081344,
        (seq_t)34602480
    };
    if (n < 6)
        return base[n];

    /* Keep the last five values for the recurrence */
    seq_t last[5];
    /* For n = 6 we need a(5)..a(1) as the window */
    last[0] = base[5]; /* a(n-1) */
    last[1] = base[4]; /* a(n-2) */
    last[2] = base[3]; /* a(n-3) */
    last[3] = base[2]; /* a(n-4) */
    last[4] = base[1]; /* a(n-5) */

    for (idx_t i = 6; i <= n; ++i) {
        seq_t newv = (seq_t)31 * last[0] +
                     (seq_t)31 * last[1] +
                     (seq_t)31 * last[2] +
                     (seq_t)31 * last[3] -
                     (seq_t)496 * last[4];
        /* shift window: last[4] becomes last[3], etc. */
        last[4] = last[3];
        last[3] = last[2];
        last[2] = last[1];
        last[1] = last[0];
        last[0] = newv;
    }
    return last[0];
}
