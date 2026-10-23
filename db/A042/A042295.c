/*
 * A042295  Denominators of continued fraction convergents to sqrt(673).
 * https://oeis.org/A042295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) is the denominator of the n-th convergent of sqrt(673). The
 *            continued fraction of sqrt(673) is [25; 1, 16, 3, 5, 2, 3, 1, 1,
 *            6, 1, 5, 1, 1, 1, 1, 1, 1, 1, 1, 5, 1, 6, 1, 1, 3, 2, 5, 3, 16,
 *            1, 50, ...] with period 31
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 25, u64 n <= 41, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the first two terms directly. */
    if (n == 0) return 1;
    if (n == 1) return 1;

    /* Periodic part of the continued fraction after the leading term 25. */
    static const int a_period[31] = {
        1, 16, 3, 5, 2, 3, 1, 1, 6, 1, 5, 1, 1, 1, 1, 1, 1, 1, 1, 5,
        1, 6, 1, 1, 3, 2, 5, 3, 16, 1, 50
    };

    /* The recurrence for convergent denominators:
       q_{-2} = 1, q_{-1} = 0
       q_i   = a_i * q_{i-1} + q_{i-2}
       with a_0 = 25 and a_i for i ≥ 1 given by a_period[(i-1) % 31] */
    seq_t q_prev2 = 1;   /* q_{i-2}, initially q_0 */
    seq_t q_prev1 = 1;   /* q_{i-1}, initially q_1 */

    for (idx_t i = 2; i <= n; ++i) {
        int coef = a_period[(i - 1) % 31]; /* a_i for i ≥ 1 */
        seq_t q = (seq_t)coef * q_prev1 + q_prev2;
        q_prev2 = q_prev1;
        q_prev1 = q;
    }
    return q_prev1;
}
