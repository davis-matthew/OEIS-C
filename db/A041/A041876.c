/*
 * A041876  Numerators of continued fraction convergents to sqrt(460).
 * https://oeis.org/A041876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 16, u64 n <= 33, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for the numerators of convergents to sqrt(460)
 * Lagrange continued‑fraction algorithm:
 *   a₀ = ⌊√460⌋ = 21
 *   For k ≥ 0:
 *     m₀ = 0, d₀ = 1
 *     mᵢ₊₁ = dᵢ·aᵢ – mᵢ
 *     dᵢ₊₁ = (460 – mᵢ₊₁²) / dᵢ
 *     aᵢ₊₁ = (a₀ + mᵢ₊₁) / dᵢ₊₁
 *   Convergents satisfy
 *     p₋₂ = 0, p₋₁ = 1
 *     pᵢ = aᵢ·pᵢ₋₁ + pᵢ₋₂
 * The function returns pₙ, the numerator of the n‑th convergent
 * (with a(0) = 21 as listed in OEIS).  All arithmetic is performed
 * using the integer type seq_t; no floating‑point operations are used. */

seq_t a(idx_t n)
{
    const seq_t S   = 460;   /* constant under the square root   */
    const seq_t a0  = 21;    /* floor(sqrt(460))                 */

    /* initial terms of the continued fraction */
    seq_t m = 0;            /* m₀ */
    seq_t d = 1;            /* d₀ */
    seq_t a_cur = a0;       /* a₀ */

    /* recurrence for convergents */
    seq_t p_nm2 = 0;        /* p₋₂ */
    seq_t p_nm1 = 1;        /* p₋₁ */
    seq_t p_cur = a_cur * p_nm1 + p_nm2;  /* p₀ = 21 */

    if (n == 0) {
        return p_cur;
    }

    /* prepare for first iteration (i = 1) */
    p_nm2 = p_nm1;
    p_nm1 = p_cur;

    for (idx_t i = 1; i <= n; ++i) {
        /* compute next CF coefficient aᵢ   */
        seq_t m_next = d * a_cur - m;
        seq_t d_next = (S - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;

        /* advance state */
        m     = m_next;
        d     = d_next;
        a_cur = a_next;

        /* compute next numerator pᵢ   */
        p_cur = a_cur * p_nm1 + p_nm2;

        /* shift for next recurrence step */
        p_nm2 = p_nm1;
        p_nm1 = p_cur;
    }

    return p_cur;
}
