/*
 * A041090  Numerators of continued fraction convergents to sqrt(53).
 * https://oeis.org/A041090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of the nth convergent of the continued fraction
 *            of sqrt(53)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 35, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Partial quotients:  a0 = 7, then periodic pattern 3,1,1,3,14 repeating. */

seq_t a(idx_t n)
{
    /* recurrence: p_{-2} = 0, p_{-1} = 1, p_i = a_i * p_{i-1} + p_{i-2} */
    seq_t p_prev2 = 0;
    seq_t p_prev1 = 1;
    seq_t p_curr = 0;

    /* fixed pattern after the initial term */
    static const seq_t pattern[5] = { 3, 1, 1, 3, 14 };

    for (idx_t i = 0; i <= n; ++i) {
        seq_t a_i;
        if (i == 0) {
            a_i = 7;
        } else {
            /* index in the period: 0..4 */
            idx_t idx = (i - 1) % 5;
            a_i = pattern[(int)idx];
        }

        /* update recurrence */
        p_curr = a_i * p_prev1 + p_prev2;

        /* shift for next step */
        p_prev2 = p_prev1;
        p_prev1 = p_curr;
    }

    return p_curr;
}
