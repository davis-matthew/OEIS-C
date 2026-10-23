/*
 * A041169  Denominators of continued fraction convergents to sqrt(94).
 * https://oeis.org/A041169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   Denominators of convergents for sqrt(94). Recurrence: q_{-1}=0,
 *            q_0=1; q_k = a_k*q_{k-1} + q_{k-2}, where the continued‑fraction
 *            terms for k≥1 are (1,2,3,1,1,5,1,8,1,5,1,1,3,2,1,18) repeating
 *            with period 16
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 47, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Periodic continued‑fraction terms for sqrt(94), excluding a0. */
    static const seq_t period_terms[] = {1, 2, 3, 1, 1, 5, 1, 8,
                                         1, 5, 1, 1, 3, 2, 1, 18};
    const idx_t period = sizeof(period_terms) / sizeof(period_terms[0]);

    seq_t q_prev2 = 0;   /* q_{i-2}  */
    seq_t q_prev1 = 1;   /* q_{i-1}  */
    seq_t q = 0;         /* q_i      */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t a_i = period_terms[(i - 1) % period];
        q = a_i * q_prev1 + q_prev2;
        q_prev2 = q_prev1;
        q_prev1 = q;
    }
    return q_prev1;
}
