/*
 * A042794  Numerators of continued fraction convergents to sqrt(928).
 * https://oeis.org/A042794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n‑th convergent to sqrt(928).
 *            Recurrence: p_{-2}=0, p_{-1}=1; p_n = a_n * p_{n-1} + p_{n-2},
 *            where a_0 = 30 and for n ≥ 1, the partial quotients a_n repeat
 *            the period [2,6,3,1,1,1,8,15,8,1,1,1,3,6,2,60]
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Partial quotients for sqrt(928) after the initial term. */
    static const unsigned period[16] = {
        2, 6, 3, 1, 1, 1, 8, 15,
        8, 1, 1, 1, 3, 6, 2, 60
    };

    seq_t p_prevprev = 0; /* p_{-2} */
    seq_t p_prev     = 1; /* p_{-1} */
    seq_t q_prevprev = 1; /* q_{-2} (unused after the loop) */
    seq_t q_prev     = 0; /* q_{-1} (unused after the loop) */

    seq_t cur_p = 0;
    seq_t cur_q = 0;

    for (idx_t i = 0; i <= n; ++i) {
        seq_t a_k;
        if (i == 0) {
            a_k = (seq_t)30;
        } else {
            unsigned idx = (unsigned)((i - 1) % 16);
            a_k = (seq_t)period[idx];
        }

        cur_p = a_k * p_prev + p_prevprev;
        cur_q = a_k * q_prev + q_prevprev;

        /* Shift for next iteration */
        p_prevprev = p_prev;
        p_prev = cur_p;
        q_prevprev = q_prev;
        q_prev = cur_q;
    }

    return cur_p;
}
