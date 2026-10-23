/*
 * A042409  Denominators of continued fraction convergents to sqrt(732).
 * https://oeis.org/A042409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A042409: Denominators of convergents of the continued fraction of sqrt(732) */
/* a(0)=1, a(1)=18, a(2)=973, ... */

seq_t a(idx_t n)
{
    const idx_t D = 732;
    const seq_t a0 = 27;            /* floor(sqrt(732)) */

    /* Continued‑fraction variables */
    seq_t m   = 0;                  /* initial m_0 */
    seq_t d   = 1;                  /* initial d_0 */
    seq_t a_c = a0;                 /* current coefficient a_k */

    /* Convergent recurrence (numerator/denominator) */
    seq_t p_prev2 = 0;
    seq_t p_prev1 = 1;
    seq_t q_prev2 = 1;
    seq_t q_prev1 = 0;

    idx_t i;
    for (i = 0; i <= n; ++i) {
        /* Compute current convergent */
        seq_t p_cur = a_c * p_prev1 + p_prev2;
        seq_t q_cur = a_c * q_prev1 + q_prev2;

        if (i == n) return q_cur;   /* desired term reached */

        /* Shift indices for next iteration */
        p_prev2 = p_prev1;
        p_prev1 = p_cur;
        q_prev2 = q_prev1;
        q_prev1 = q_cur;

        /* Generate next continued‑fraction coefficient */
        seq_t m_next = d * a_c - m;
        seq_t d_next = ((seq_t)D - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;

        m   = m_next;
        d   = d_next;
        a_c = a_next;
    }

    /* Should never reach here */
    return 0;
}
