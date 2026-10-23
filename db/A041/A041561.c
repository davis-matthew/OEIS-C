/*
 * A041561  Denominators of continued fraction convergents to sqrt(298).
 * https://oeis.org/A041561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   Denominators of convergents to sqrt(298). a(n) is the
 *            denominator of the n-th convergent in the simple continued
 *            fraction expansion of sqrt(298). The recurrence uses the
 *            standard continued‑fraction formulas and the recurrence for
 *            convergents: m0 = 0, d0 = 1, a0 = floor(sqrt(298)) = 17 mk+1 =
 *            dk *...
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t D   = 298;
    const idx_t a0  = 17;
    idx_t m = 0;
    idx_t d = 1;
    idx_t a = a0;

    seq_t p_prevprev = 0;
    seq_t p_prev = 1;
    seq_t q_prevprev = 1;
    seq_t q_prev = 0;

    for (idx_t step = 0; step <= n; ++step) {
        seq_t p_next = (seq_t)a * p_prev + p_prevprev;
        seq_t q_next = (seq_t)a * q_prev + q_prevprev;

        if (step == n) {
            return q_next;
        }

        /* shift for next iteration */
        p_prevprev = p_prev;
        p_prev = p_next;
        q_prevprev = q_prev;
        q_prev = q_next;

        /* compute next partial quotient */
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;
    }

    return 0; /* unreachable */
}
