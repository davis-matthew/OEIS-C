/*
 * A042772  Numerators of continued fraction convergents to sqrt(917).
 * https://oeis.org/A042772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0)=30; for n≥1, a(n)=a_k*p_{k‑1}+p_{k‑2} * where p_{-1}=1,
 *            p_0=30 and a_k are the continued‑fraction * coefficients of √917
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    const idx_t D   = (idx_t)917;   /* discriminant */
    const idx_t a0  = (idx_t)30;    /* floor(√917) */

    /* special case */
    if (n == (idx_t)0) return (seq_t)a0;

    /* states for continued‑fraction expansion of √D */
    idx_t m = (idx_t)0;
    idx_t d = (idx_t)1;
    idx_t a_coef = a0;

    /* recurrence for convergents: p_{-1}=1, p_0=a0 */
    seq_t p_prev  = (seq_t)1;   /* p_{k‑2} */
    seq_t p_curr  = (seq_t)a0;  /* p_{k‑1} */

    for (idx_t k = (idx_t)1; k <= n; ++k) {
        /* step of the continued‑fraction algorithm */
        m   = d * a_coef - m;
        d   = (D - m * m) / d;
        a_coef = (a0 + m) / d;

        /* update convergents */
        seq_t p_next = a_coef * p_curr + p_prev;
        p_prev = p_curr;
        p_curr = p_next;
    }

    return p_curr;
}
