/*
 * A041366  Numerators of continued fraction convergents to sqrt(198).
 * https://oeis.org/A041366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = Numerators of continued fraction convergents to sqrt(198)
 *            Recurrence: h[-2] = 0, h[-1] = 1 a[0] = 14 a[1] = 14, a[2] = 28,
 *            a[3] = 14, a[4] = 28, … h[n] = a[n] * h[n-1] + h[n-2]
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t h_prev2 = 0;     /* h[-2] */
    seq_t h_prev1 = 1;     /* h[-1] */
    seq_t h_cur   = 0;

    for (idx_t i = 0; i <= n; ++i) {
        seq_t term;
        if (i == 0 || (i & 1))          /* a[0] = 14, a[odd] = 14, a[even>=2] = 28 */
            term = 14;
        else
            term = 28;

        h_cur = term * h_prev1 + h_prev2;

        if (i == n)
            return h_cur;

        h_prev2 = h_prev1;
        h_prev1 = h_cur;
    }
    return h_cur;   /* unreachable, but keeps compiler happy */
}
