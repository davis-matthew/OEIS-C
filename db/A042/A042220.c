/*
 * A042220  Numerators of continued fraction convergents to sqrt(636).
 * https://oeis.org/A042220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerators of the convergents of the continued fraction
 *            for sqrt(636)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 15, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* special case for n = 0 */
    if (n == (idx_t)0)
        return (seq_t)25;           /* floor(sqrt(636)) = 25 */

    /* Constants for sqrt(636) */
    const idx_t D   = (idx_t)636;   /* value under the square root        */
    const idx_t a0  = (idx_t)25;    /* floor(sqrt(636))                    */

    /* Variables for the continued‑fraction algorithm */
    idx_t m = (idx_t)0;            /* remainder term m_n                 */
    idx_t d = (idx_t)1;            /* denominator term d_n                */
    idx_t a = a0;                  /* current partial quotient a_n        */

    /* Recurrence for convergent numerators:
     * p_{-2} = 0,  p_{-1} = 1
     * p_k   = a_k * p_{k-1} + p_{k-2}
     */
    seq_t pkm2 = (seq_t)0;      /* p_{k-2} */
    seq_t pkm1 = (seq_t)1;      /* p_{k-1} */
    seq_t p_k  = (seq_t)0;

    for (idx_t k = (idx_t)0; k <= n; ++k)
    {
        /* Compute the k‑th numerator */
        p_k = (seq_t)a * pkm1 + pkm2;
        if (k == n)
            return p_k;

        /* Advance to the next partial quotient
         *   m_{k+1} = d_k * a_k - m_k
         *   d_{k+1} = (D - m_{k+1}^2) / d_k
         *   a_{k+1} = ⌊ (a0 + m_{k+1}) / d_{k+1} ⌋
         */
        m = d * a - m;
        d = (D - (m * m)) / d;
        a = (a0 + m) / d;

        /* Shift the previous two numerators for the next step */
        pkm2 = pkm1;
        pkm1 = p_k;
    }

    /* Unreachable, but keeps the compiler happy */
    return (seq_t)0;
}
