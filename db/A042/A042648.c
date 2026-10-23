/*
 * A042648  Numerators of continued fraction convergents to sqrt(854).
 * https://oeis.org/A042648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents for sqrt(854)
   Uses the recurrence p_k = a_k * p_{k-1} + p_{k-2}
   where the a_k are the terms of the continued fraction of sqrt(854). */

seq_t a(idx_t n)
{
    if (n == 0)
    {
        return (seq_t)29;
    }

    const idx_t D = 854;          /* the radicand */
    idx_t a0      = 29;           /* a_0 = floor(sqrt(D)) */
    idx_t m       = 0;            /* m_0 */
    idx_t d       = 1;            /* d_0 */
    idx_t a_curr  = a0;           /* current coefficient a_k */

    seq_t p_prev2 = (seq_t)1;     /* p_{-1} */
    seq_t p_prev1 = (seq_t)29;    /* p_0 */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* next m, d and a_k according to the classic algorithm */
        m = d * a_curr - m;
        d = (D - m * m) / d;
        a_curr = (a0 + m) / d;

        /* recurrence for the numerator of the convergent */
        seq_t p = a_curr * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = p;
    }

    return p_prev1;
}
