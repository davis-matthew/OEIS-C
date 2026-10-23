/*
 * A041384  Numerators of continued fraction convergents to sqrt(207).
 * https://oeis.org/A041384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n-th convergent of sqrt(207) Recurrence:
 *            a(0)=14, a(1)=29, a(n)=c(n)*a(n-1)+a(n-2) for n≥2, where c(n)
 *            are the periodic continued‑fraction coefficients of sqrt(207)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 22, u64 n <= 45, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t D = 207;        /* D is the square‑free integer */
    const idx_t a0 = 14;        /* floor(sqrt(207)) */

    if (n == 0)
        return (seq_t)a0;

    /* continued fraction variables */
    idx_t m   = 0;              /* previous m */
    idx_t d   = 1;              /* previous d */
    idx_t a_coeff = a0;         /* coefficient a_n, starting with a0 */

    /* previous two numerators of convergents */
    seq_t p_prev2 = (seq_t)1;   /* p_{-1} = 1  */
    seq_t p_prev1 = (seq_t)a0;  /* p_0     = a0 */

    for (idx_t i = 1; i <= n; ++i) {
        /* next coefficient a_i */
        m = d * a_coeff - m;
        d = (D - m * m) / d;
        a_coeff = (a0 + m) / d;

        /* compute p_i = a_i * p_{i-1} + p_{i-2} */
        seq_t coeff   = (seq_t)a_coeff;
        seq_t p_curr = coeff * p_prev1 + p_prev2;

        if (i == n)
            return p_curr;

        /* shift for next step */
        p_prev2 = p_prev1;
        p_prev1 = p_curr;
    }
    return (seq_t)0; /* unreachable for n ≥ 0 */
}
