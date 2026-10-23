/*
 * A042125  Denominators of continued fraction convergents to sqrt(587).
 * https://oeis.org/A042125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 20, u64 n <= 41, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = denominator of the nth convergent of the continued
   fraction for sqrt(587) (hence the sequence of denominations of the
   square root).  */
seq_t a(idx_t n)
{
    /* Period of the continued fraction for sqrt(587); a[0] is a0 = 24. */
    static const int period[14] = {4, 2, 1, 1, 1, 1, 23, 1,
                                   1, 1, 1, 2, 4, 48};

    /* p_{-2}=0, p_{-1}=1; q_{-2}=1, q_{-1}=0 */
    seq_t p_m2 = 0, p_m1 = 1;
    seq_t q_m2 = 1, q_m1 = 0;

    idx_t i;
    for (i = 0; i <= n; ++i) {
        int a_i;
        if (i == 0) {
            a_i = 24;                 /* a0 */
        } else {
            a_i = period[(int)((i - 1) % 14)];
        }

        /* Compute p_i = a_i * p_{i-1} + p_{i-2}
           and q_i = a_i * q_{i-1} + q_{i-2} */
        seq_t p_i = (seq_t)a_i * p_m1 + p_m2;
        seq_t q_i = (seq_t)a_i * q_m1 + q_m2;

        /* Shift for next iteration */
        p_m2 = p_m1; p_m1 = p_i;
        q_m2 = q_m1; q_m1 = q_i;
    }

    return q_m1;   /* q_n is the required denominator */
}
