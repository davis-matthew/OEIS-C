/*
 * A084844  Denominators of the continued fraction n + 1/(n + 1/...) [n
 *          times].
 * https://oeis.org/A084844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = q_n where q_0 = 0, q_1 = 1 and for k ≥ 2, q_k = n *
 *            q_{k-1} + q_{k-2}. This recurrence gives the denominator of the
 *            simple continued fraction consisting of n repeated n times
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) = 1. */
    if (n <= 1) {
        return (seq_t)1;
    }

    seq_t q0 = (seq_t)0;          /* q_0 */
    seq_t q1 = (seq_t)1;          /* q_1 */
    seq_t q2;

    for (idx_t i = 2; i <= n; ++i) {
        q2 = (seq_t)n * q1 + q0;  /* q_i = n*q_{i-1} + q_{i-2} */
        q0 = q1;
        q1 = q2;
    }

    return q1;                    /* q_n */
}
