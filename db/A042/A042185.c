/*
 * A042185  Denominators of continued fraction convergents to sqrt(617).
 * https://oeis.org/A042185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 18, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = denominator of the n-th convergent of the continued fraction of sqrt(617) */

seq_t a(idx_t n)
{
    /* Periodic part of the continued fraction (excluding the initial term a0 = 24) */
    static const seq_t period[15] = {
        1, 5, 4, 2, 1, 6, 2, 2, 6, 1, 2, 4, 5, 1, 48
    };

    /* Continued‑fraction recurrence */
    seq_t p_minus2 = 0, p_minus1 = 1;   /* p_{-2}, p_{-1}   */
    seq_t q_minus2 = 1, q_minus1 = 0;   /* q_{-2}, q_{-1}   */
    seq_t p_cur, q_cur;
    seq_t a_k;

    for (idx_t k = 0; k <= n; ++k) {
        if (k == 0)
            a_k = (seq_t)24;                 /* a0 = floor(sqrt(617)) */
        else
            a_k = period[(k - 1) % 15];     /* periodic coefficients */

        p_cur = a_k * p_minus1 + p_minus2;
        q_cur = a_k * q_minus1 + q_minus2;

        if (k == n)
            return q_cur;                    /* desired denominator */

        /* shift recurrence state */
        p_minus2 = p_minus1;
        p_minus1 = p_cur;
        q_minus2 = q_minus1;
        q_minus1 = q_cur;
    }
    return 0;  /* should never happen */
}
