/*
 * A041167  Denominators of continued fraction convergents to sqrt(93).
 * https://oeis.org/A041167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the nth convergent of sqrt(93)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 23, u64 n <= 44, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Notation:
       D  – modulus in the continued‑fraction algorithm (93)
       a_coeff – current partial quotient (a0, a1, …)
       m, d – auxiliary parameters of the algorithm
       p, q – numerator and denominator of the convergent
    */
    const seq_t D = (seq_t)93;          /* sqrt(93) ≈ 9.643... */
    seq_t a_coeff = (seq_t)9;           /* floor(sqrt(93)) = a0 */
    seq_t m = (seq_t)0;
    seq_t d = (seq_t)1;

    /* Convergents start with p[-2]=0, p[-1]=1; q[-2]=1, q[-1]=0 */
    seq_t p_im2 = (seq_t)0, p_im1 = (seq_t)1;
    seq_t q_im2 = (seq_t)1, q_im1 = (seq_t)0;

    for (idx_t k = 0; k <= n; ++k)
    {
        /* Compute kth convergent numerator and denominator */
        seq_t p_k = a_coeff * p_im1 + p_im2;
        seq_t q_k = a_coeff * q_im1 + q_im2;

        if (k == n)
            return q_k;                /* denominator is the required term */

        /* Update parameters for the next partial quotient */
        m = d * a_coeff - m;
        d = (D - m * m) / d;
        a_coeff = ((seq_t)9 + m) / d;   /* sqrt(93) = 9 */

        /* Shift previous two terms for the next iteration */
        p_im2 = p_im1;  p_im1 = p_k;
        q_im2 = q_im1;  q_im1 = q_k;
    }

    /* Should never reach here */
    return (seq_t)0;
}
