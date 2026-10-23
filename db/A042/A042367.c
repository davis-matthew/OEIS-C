/*
 * A042367  Denominators of continued fraction convergents to sqrt(710).
 * https://oeis.org/A042367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the nth convergent of the continued
 *            fraction expansion of sqrt(710)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 23, u64 n <= 47, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Period of the continued fraction of sqrt(710): [26;
       1,1,1,4,1,1,1,52] */
    static const seq_t period[8] = {1, 1, 1, 4, 1, 1, 1, 52};

    if (n == 0)
        return (seq_t)1;   /* q_0 = 1 */

    seq_t p_prev2 = (seq_t)1,      /* p_{-1}  */
          p_prev1 = (seq_t)26;     /* p_0     */
    seq_t q_prev2 = (seq_t)0,      /* q_{-1}  */
          q_prev1 = (seq_t)1;      /* q_0     */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t a_k = period[(k - 1) % 8];
        seq_t p = a_k * p_prev1 + p_prev2;
        seq_t q = a_k * q_prev1 + q_prev2;
        p_prev2 = p_prev1;  p_prev1 = p;
        q_prev2 = q_prev1;  q_prev1 = q;
    }

    return q_prev1;   /* q_n */
}
