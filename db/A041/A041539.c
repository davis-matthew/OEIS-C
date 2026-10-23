/*
 * A041539  Denominators of continued fraction convergents to sqrt(286).
 * https://oeis.org/A041539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 17, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A041539:  Denominators of continued fraction convergents to sqrt(286)
    Formula: a(n) = 1123670*a(n-10) - a(n-20)  for n > 19
    Initial values: a(0)=1, a(1)=1, a(2)=11, a(3)=34, a(4)=113, a(5)=260,
                    a(6)=893, a(7)=2939, ...  (computed by the algorithm below)   */
seq_t a(idx_t n)
{
    /* constant square root of 286 */
    const seq_t D    = (seq_t)286;      /* D = sqrt(286)^2 */
    const seq_t a0   = (seq_t)16;       /* floor(sqrt(286)) */

    /* special cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    /* continued‑fraction variables for sqrt(286) */
    seq_t m   = (seq_t)0;   /* m_k */
    seq_t d   = (seq_t)1;   /* d_k */
    seq_t a_k = a0;         /* a_k (partial quotient) */

    /* convergent denominators: q_{-1}=0, q_0=1 */
    seq_t q_prevprev = (seq_t)0;
    seq_t q_prev     = (seq_t)1;
    seq_t q_curr;

    /* compute denominators up to a(n) */
    for (idx_t step = 0; step < n; ++step)
    {
        /* compute next m, d, a values */
        seq_t m_next = d * a_k - m;
        seq_t d_next = (D - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;

        /* compute next convergent denominator */
        q_curr = a_next * q_prev + q_prevprev;

        /* advance state */
        m          = m_next;
        d          = d_next;
        a_k        = a_next;
        q_prevprev = q_prev;
        q_prev     = q_curr;
    }

    /* q_prev now holds a(n) */
    return q_prev;
}
