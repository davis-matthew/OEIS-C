/*
 * A042503  Denominators of continued fraction convergents to sqrt(779).
 * https://oeis.org/A042503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator q_n of the n-th convergent of sqrt(779) via
 *            the continued‑fraction recurrence with a0 = 27 and period [1,
 *            10, 5, 2, 27, 2, 5, 10, 1, 54]
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* periodic part of the continued fraction of sqrt(779) */
    enum { PERIOD = 10 };
    static const seq_t B[PERIOD] = {
        (seq_t)1, (seq_t)10, (seq_t)5, (seq_t)2, (seq_t)27,
        (seq_t)2, (seq_t)5, (seq_t)10, (seq_t)1, (seq_t)54
    };

    /* initial values for the recurrence q_{-2}=1, q_{-1}=0 */
    seq_t q_prev2 = (seq_t)1;  /* q_{-2} */
    seq_t q_prev1 = (seq_t)0;  /* q_{-1} */

    for (idx_t i = (idx_t)0; i <= n; ++i)
    {
        seq_t a_i;
        if (i == (idx_t)0)
            a_i = (seq_t)27; /* a_0 */
        else
            a_i = B[(idx_t)(i - 1) % (idx_t)PERIOD]; /* periodic block */

        /* recurrence: q_i = a_i * q_{i-1} + q_{i-2} */
        seq_t q = a_i * q_prev1 + q_prev2;

        /* shift for next iteration */
        q_prev2 = q_prev1;
        q_prev1 = q;
    }

    return q_prev1;  /* q_n */
}
