/*
 * A042344  Numerators of continued fraction convergents to sqrt(699).
 * https://oeis.org/A042344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n‑th convergent to √699
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: p_{-2}=0, p_{-1}=1, p_k = a_k * p_{k-1} + p_{k-2}   */
/* a_0 = 26, a_{k>0} are the periodic partial quotients of √699 with period 14. */

seq_t a(idx_t n)
{
    const idx_t a0 = 26;
    /* periodic part a₁ .. a₁₄ of √699 (a₁₄ = 2·a₀). */
    const seq_t period[14] =
    {
        2, 3, 1, 1, 2, 1, 25, 1, 2, 1, 1, 3, 2, 52
    };

    seq_t p_minus2 = 0;   /* p_{-2} */
    seq_t p_minus1 = 1;   /* p_{-1} */
    seq_t p_i      = 0;   /* p_k */

    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t a_i = (i == 0) ? (seq_t)a0 : period[(i - 1) % 14];
        p_i = a_i * p_minus1 + p_minus2;
        p_minus2 = p_minus1;
        p_minus1 = p_i;
    }
    return p_i;
}
