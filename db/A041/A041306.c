/*
 * A041306  Numerators of continued fraction convergents to sqrt(166).
 * https://oeis.org/A041306
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, cofr, easy
 * Formula:   a(n) = numerator of the nth convergent of the continued fraction
 *            expansion of √166. Recurrence: p_-2 = 0, p_-1 = 1 a_0 = 12 a_k
 *            for k ≥ 1 are periodic with period 22:
 *            (1,7,1,1,1,2,4,1,3,2,12,2,3,1,4,2,1,1,1,7,1,24) p_k = a_k *
 *            p_{k-1} + p_{k-2} for k ≥ 0 a(0) = p_0 = 12, a(n) = p_n
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)12;

    static const seq_t PERIOD[22] = {
        1, 7, 1, 1, 1, 2, 4, 1, 3, 2,
        12, 2, 3, 1, 4, 2, 1, 1, 1, 7,
        1, 24
    };

    seq_t p_prev2 = 0;         /* corresponds to p_{-2}   */
    seq_t p_prev1 = 1;         /* corresponds to p_{-1}   */
    seq_t p_curr  = 12;        /* p_0  */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t a_k;
        if (k <= 22)
            a_k = PERIOD[k - 1];
        else
            a_k = PERIOD[(k - 1) % 22];

        p_prev2 = p_prev1;
        p_prev1 = p_curr;
        p_curr  = a_k * p_prev1 + p_prev2;
    }
    return p_curr;
}
