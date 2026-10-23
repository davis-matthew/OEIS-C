/*
 * A042411  Denominators of continued fraction convergents to sqrt(733).
 * https://oeis.org/A042411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the nth convergent to sqrt(733).
 *            Continued‑fraction coefficients after the integer part are
 *            periodic with period 5: [13, 1, 1, 13, 54]. Recurrence:
 *            q_{-1}=0, q_0=1, q_k = c_k*q_{k-1}+q_{k-2}
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 23, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Periodic coefficients of sqrt(733) after the integer part */
    static const seq_t cf[5] = {13, 1, 1, 13, 54};
    /* q_{-1} = 0, q_0 = 1  */
    seq_t q_prev2 = 0;            /* q_{k-2} */
    seq_t q_prev  = 1;            /* q_{k-1} */

    /* Iterate up to n to obtain the denominator q_n */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t coeff = cf[(i - 1) % 5];
        seq_t q_new = coeff * q_prev + q_prev2;
        q_prev2 = q_prev;
        q_prev  = q_new;
    }
    return q_prev;
}
