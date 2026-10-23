/*
 * A041704  Numerators of continued fraction convergents to sqrt(372).
 * https://oeis.org/A041704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of convergents to sqrt(372).
   Recurrence:
     p_{-2} = 0, p_{-1} = 1,
     p_n    = a_n * p_{n-1} + p_{n-2}
   where
     a_0 = 19
     a_k for k ≥ 1 cycles through [3, 2, 12, 2, 3, 38] (the continued‑fraction period of sqrt(372)). */

seq_t a(idx_t n)
{
    /* period of the continued–fraction expansion of sqrt(372) */
    static const int period[] = {3, 2, 12, 2, 3, 38};
    const unsigned period_len = sizeof(period) / sizeof(period[0]);

    seq_t p_prevprev = 0; /* p_{-2} */
    seq_t p_prev    = 1;  /* p_{-1} */

    for (idx_t i = 0; i <= n; ++i) {
        int ai;
        if (i == 0)
            ai = 19;                       /* a_0 */
        else
            ai = period[(i - 1) % period_len]; /* a_i for i ≥ 1 */

        seq_t p = (seq_t)ai * p_prev + p_prevprev;   /* p_i */

        if (i == n)
            return p;

        p_prevprev = p_prev;
        p_prev = p;
    }

    /* Unreachable, but keeps the compiler happy */
    return 0;
}
