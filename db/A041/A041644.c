/*
 * A041644  Numerators of continued fraction convergents to sqrt(341).
 * https://oeis.org/A041644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the nth convergent of sqrt(341) as a
 *            continued fraction. Continued fraction: sqrt(341) = [18;
 *            2,6,1,8,2,1,2,1,2,8,1,6,2,36] (period 14). Recurrence: p_{-2} =
 *            0, p_{-1} = 1, p_k = c_k * p_{k-1} + p_{k-2}, where c_0 = 18 and
 *            for k ≥ 1, c_k follows the periodic sequence above
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 15, u64 n <= 36, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t p_prev2 = 0;   /* p_{-2} */
    seq_t p_prev1 = 1;   /* p_{-1} */
    seq_t p = 0;         /* current numerator */

    /* Periodic part of the continued fraction after the initial term 18. */
    static const seq_t period[14] = {
        2, 6, 1, 8, 2, 1, 2, 1, 2, 8, 1, 6, 2, 36
    };
    const int period_len = 14;

    for (idx_t i = 0; i <= n; ++i) {
        seq_t coeff;
        if (i == 0) {
            coeff = (seq_t)18;
        } else {
            int pos = (int)((i - 1) % period_len);
            coeff = period[pos];
        }
        p = coeff * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = p;
    }
    return p;
}
