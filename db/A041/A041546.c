/*
 * A041546  Numerators of continued fraction convergents to sqrt(291).
 * https://oeis.org/A041546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 17.
   For n ≥ 1 let the continued‑fraction coefficients of √291 be
   c₀ = 17, c₁ = 17, c₂ = 34, and thereafter repeat 17,34.
   Then a(n) = cₙ * a(n‑1) + a(n‑2). */

seq_t a(idx_t n)
{
    /* Constant coefficients of the continued fraction of √291.  */
    const seq_t a0_coeff = (seq_t)17;  /* a₀   */
    const seq_t a1_coeff = (seq_t)17;  /* first periodic coeff */
    const seq_t a2_coeff = (seq_t)34;  /* second periodic coeff */

    /* Recurrence p₋₂ = 0, p₋₁ = 1.  */
    seq_t p_prev2 = (seq_t)0;    /* p_{i-2} */
    seq_t p_prev1 = (seq_t)1;    /* p_{i-1} */
    seq_t p_cur = 0;             /* p_{i}   */

    for (idx_t i = 0; i <= n; ++i) {
        seq_t coeff;
        if (i == 0)
            coeff = a0_coeff;
        else
            coeff = (( (i - 1) & 1 ) == 0) ? a1_coeff : a2_coeff;

        p_cur = coeff * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = p_cur;
    }

    return p_cur;
}
