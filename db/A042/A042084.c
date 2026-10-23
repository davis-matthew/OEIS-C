/*
 * A042084  Numerators of continued fraction convergents to sqrt(566).
 * https://oeis.org/A042084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = numerator of the (n+1)-th convergent of the continued
 *            fraction of sqrt(566)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 40, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is equivalent to the numerators of the convergents to sqrt(566). */
/* Recurrence: p_{-2}=0, p_{-1}=1; p_k = a_k * p_{k-1} + p_{k-2}   (a_0=23, a_k computed by the continued‑fraction algorithm) */

seq_t a(idx_t n)
{
    /* Continued fraction data for sqrt(566) */
    const idx_t D = 566u;          /* the non‑square integer */
    const idx_t a0 = 23u;          /* floor(sqrt(D)) */

    /* Previous two numerators: p_{k-2} and p_{k-1} */
    seq_t p_prev2 = 0;              /* p_{-2} = 0 */
    seq_t p_prev1 = 1;              /* p_{-1} = 1 */

    /* Current state of the continued fraction algorithm */
    idx_t m = 0;                    /* m_0 */
    idx_t d = 1;                    /* d_0 */
    idx_t a_k = a0;                 /* a_0 for k=0 */

    for (idx_t k = 0; k <= n; ++k) {
        if (k == 0) {
            /* a_0 already set */
        } else {
            /* Generate next continued‑fraction coefficient a_k */
            /* m_k = d_{k-1} * a_{k-1} - m_{k-1} */
            idx_t temp = d * a_k - m;
            m = temp;
            /* d_k = (D - m_k^2) / d_{k-1} */
            d = (D - m * m) / d;
            /* a_k = floor((a0 + m_k) / d_k) */
            a_k = (a0 + m) / d;
        }

        /* Compute the k-th numerator using the recurrence */
        seq_t p = (seq_t)a_k * p_prev1 + p_prev2;

        if (k == n) {
            return p;
        }

        /* Advance p_prev2 and p_prev1 for next iteration */
        p_prev2 = p_prev1;
        p_prev1 = p;
    }

    /* Should never be reached */
    return 0;
}
