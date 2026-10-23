/*
 * A042327  Denominators of continued fraction convergents to sqrt(690).
 * https://oeis.org/A042327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the nth convergent to sqrt(690)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 17, u64 n <= 35, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Periodic part of the continued fraction of sqrt(690) after the
     * initial term a0 = 26:  [3,1,2,1,3,52]. */
    static const seq_t period[] = { (seq_t)3, (seq_t)1, (seq_t)2,
                                   (seq_t)1, (seq_t)3, (seq_t)52 };
    const idx_t period_len = 6;

    /* Recurrence for convergents:
     * p[-2] = 0, p[-1] = 1
     * q[-2] = 1, q[-1] = 0
     * p_k   = a_k * p_{k-1} + p_{k-2}
     * q_k   = a_k * q_{k-1} + q_{k-2}
     * The denominator of the k‑th convergent is q_k.  */
    seq_t p_prev2 = (seq_t)0,  p_prev1 = (seq_t)1;
    seq_t q_prev2 = (seq_t)1,  q_prev1 = (seq_t)0;
    seq_t p, q;
    for (idx_t k = (idx_t)0; k <= n; ++k) {
        seq_t a_k;
        if (k == (idx_t)0) {
            a_k = (seq_t)26;          /* a0 = floor(sqrt(690)) */
        } else {
            a_k = period[(k - (idx_t)1) % period_len];
        }
        p = a_k * p_prev1 + p_prev2;
        q = a_k * q_prev1 + q_prev2;
        /* shift for next step */
        p_prev2 = p_prev1;
        p_prev1 = p;
        q_prev2 = q_prev1;
        q_prev1 = q;
    }
    return q_prev1;
}
