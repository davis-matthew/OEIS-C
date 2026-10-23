/*
 * A040397  Continued fraction for sqrt(418).
 * https://oeis.org/A040397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = continued fraction coefficient of sqrt(418)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the standard algorithm for computing the periodic
 * continued‑fraction expansion of sqrt(418).  The first coefficient
 * a(0) is the integer part floor(sqrt(418)) = 20, and the remaining
 * terms repeat a period of length 6: 2,4,20,4,2,40. */

seq_t a(idx_t n)
{
    /* Static cache for the periodic part */
    static seq_t period[64];
    static idx_t period_len = 0;          /* number of coefficients stored */

    const seq_t a0 = 20;                  /* floor(sqrt(418)) */

    /* a(0) is the first term */
    if (n == 0)
        return a0;

    /* Compute the period once */
    if (period_len == 0) {
        seq_t m = 0;
        seq_t d = 1;
        seq_t a = a0;

        /* Generate coefficients until the special end term 2*a0 is reached */
        do {
            /* m_{k+1} = d_k * a_k - m_k      */
            idx_t tmp_m = ((idx_t)d * (idx_t)a) - (idx_t)m;
            m = (seq_t)tmp_m;

            /* d_{k+1} = (418 - m^2) / d_k    */
            idx_t tmp_d = ((idx_t)418 - (idx_t)m * (idx_t)m) / (idx_t)d;
            d = (seq_t)tmp_d;

            /* a_{k+1} = (a0 + m) / d         */
            idx_t tmp_a = ((idx_t)a0 + (idx_t)m) / (idx_t)d;
            a = (seq_t)tmp_a;

            period[period_len++] = a;
        } while (a != 2 * a0);   /* 2*a0 = 40, the period termination */
    }

    /* Return the n‑th term using the cached period (n≥1) */
    idx_t idx = (n - 1) % period_len;
    return period[idx];
}
