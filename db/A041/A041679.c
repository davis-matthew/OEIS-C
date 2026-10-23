/*
 * A041679  Denominators of continued fraction convergents to sqrt(358).
 * https://oeis.org/A041679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the n-th convergent of sqrt(358)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 44, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t D = 358;          /* Constant under the square root */
    const int a0 = 18;            /* floor(sqrt(358)) */
    idx_t m = 0;                  /* Initial m_0 */
    idx_t d = 1;                  /* Initial d_0 */
    int a = a0;                   /* Current partial quotient a_k */

    /* Convergent recurrence: q_{-2} = 1, q_{-1} = 0 */
    seq_t q_im2 = 1;              /* q_{-2} */
    seq_t q_im1 = 0;              /* q_{-1} */
    seq_t q = 0;                  /* Current q_k */

    for (idx_t k = 0; k <= n; ++k) {
        /* Compute q_k = a_k * q_{k-1} + q_{k-2} */
        q = (seq_t)a * q_im1 + q_im2;

        /* If this is the requested index, return the result */
        if (k == n) break;

        /* Prepare next partial quotient a_{k+1} using CF recurrence */
        idx_t m_next = d * a - m;                     /* m_{k+1} */
        idx_t d_next = (D - m_next * m_next) / d;     /* d_{k+1} */
        int a_next = (a0 + m_next) / d_next;          /* a_{k+1} */

        /* Shift variables for next iteration */
        m = m_next;
        d = d_next;
        a = a_next;

        /* Update previous q values for next step */
        q_im2 = q_im1;
        q_im1 = q;
    }

    return q;
}
