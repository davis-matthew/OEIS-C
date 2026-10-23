/*
 * A041883  Denominators of continued fraction convergents to sqrt(463).
 * https://oeis.org/A041883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0)=1; for n≥1 let cₙ be the n‑th continued‑fraction
 *            coefficient of sqrt(463). Then a(n)=cₙ·a(n‑1)+a(n‑2) with a(0)=1
 *            and a(1)=1
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 22, u64 n <= 44, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* constants for sqrt(463) */
    const idx_t D  = 463;
    const idx_t a0 = 21;

    if (n == 0) return 1;

    idx_t m = 0, d = 1;
    idx_t coeff = a0;          /* a₀ initially, will become a₁, a₂, ... */

    seq_t q_prev = 0;  /* q_{-1} */
    seq_t q_curr = 1;  /* q_0   */
    seq_t q_next;

    for (idx_t k = 1; k <= n; ++k) {
        /* compute next continued‑fraction coefficient */
        m = d * coeff - m;
        d = (D - m * m) / d;
        coeff = (a0 + m) / d;

        /* recurrence for denominators */
        q_next = (seq_t)coeff * q_curr + q_prev;
        q_prev = q_curr;
        q_curr = q_next;
    }

    return q_curr;
}
