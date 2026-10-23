/*
 * A042083  Denominators of continued fraction convergents to sqrt(565).
 * https://oeis.org/A042083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: Denominators qₙ of convergents to √565 satisfy
 *   q₋₁ = 0,  q₀ = 1
 *   qₙ   = aₙ·qₙ₋₁ + qₙ₋₂   for n ≥ 1
 * where aₙ are the continued‑fraction coefficients of √565
 * (a₀ = ⌊√565⌋ = 23). */
seq_t a(idx_t n)
{
    /* D = 565, a₀ = ⌊√565⌋ = 23 */
    const seq_t D  = (seq_t)565;
    const seq_t a0 = (seq_t)23;

    if (n == 0) return (seq_t)1;          /* q₀ */

    /* state for the continued‑fraction expansion */
    seq_t m   = (seq_t)0;
    seq_t d   = (seq_t)1;
    seq_t a_c = a0;                        /* current aᵢ value */

    /* recurrence variables: qₙ₋₂ , qₙ₋₁ , qₙ */
    seq_t q_prevprev = (seq_t)0;           /* q₋₁ */
    seq_t q_prev     = (seq_t)1;           /* q₀ */
    seq_t q_curr     = (seq_t)0;

    /* iterate up to n */
    for (idx_t i = 1; i <= n; ++i) {
        /* compute next continued‑fraction coefficient */
        seq_t m_next = d * a_c - m;
        seq_t d_next = (D - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;

        /* update convergent denominator */
        q_curr = a_next * q_prev + q_prevprev;

        /* shift for next iteration */
        q_prevprev = q_prev;
        q_prev     = q_curr;

        /* advance state for continued fraction */
        m   = m_next;
        d   = d_next;
        a_c = a_next;
    }

    return q_curr;   /* qₙ */
}
