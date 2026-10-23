/*
 * A042073  Denominators of continued fraction convergents to sqrt(560).
 * https://oeis.org/A042073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 19, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A042073 –  Denominators of continued‑fraction convergents to sqrt(560)
   The function implements the standard recurrence for convergents of a quadratic irrational.
   Let a₀ = floor(sqrt(560)) = 23.  The continued‑fraction of sqrt(560) is 
   [23; 1,1,1,46,1,1,1,46,...] so the partial quotients aₙ for n≥1 repeat
   the block (1,1,1,46).  The convergents (pₙ,qₙ) satisfy:
        pₙ = aₙ·pₙ₋₁ + pₙ₋₂,   qₙ = aₙ·qₙ₋₁ + qₙ₋₂
   with p₋₂ = 0, p₋₁ = 1, q₋₂ = 1, q₋₁ = 0.
   The required value a(n) is the denominator qₙ.                                     */

seq_t a(idx_t n)
{
    /* 560 is not a perfect square; its continued‑fraction expansion has
       period 4:  [23; 1,1,1,46] repeating. */
    const idx_t D = 560;          /* the radicand   */
    const idx_t a0 = 23;          /* floor(sqrt(560)) */

    /* States for continued‑fraction generation */
    idx_t m = 0;                  /* initial m */
    idx_t d = 1;                  /* initial d */
    idx_t a = a0;                 /* initial partial quotient */

    /* States for convergent recurrence */
    seq_t p_minus2 = 0,   p_minus1 = 1;
    seq_t q_minus2 = 1,   q_minus1 = 0;

    for (idx_t i = 0; i <= n; ++i) {
        /* compute next convergent using current partial quotient a */
        seq_t p = (seq_t)a * p_minus1 + p_minus2;
        seq_t q = (seq_t)a * q_minus1 + q_minus2;

        if (i == n)
            return q;              /* denominator of the n‑th convergent */

        /* shift recurrence states for next step */
        p_minus2 = p_minus1;  p_minus1 = p;
        q_minus2 = q_minus1;  q_minus1 = q;

        /* update continued‑fraction state for next a */
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;
    }

    /* should never reach here */
    return 0;
}
