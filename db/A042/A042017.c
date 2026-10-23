/*
 * A042017  Denominators of continued fraction convergents to sqrt(532).
 * https://oeis.org/A042017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the nth convergent to sqrt(532) (a(0)=1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* partial quotients of the continued fraction of sqrt(532) after a0=23 */
    static const unsigned a_period[10] = {15, 2, 1, 4, 2, 4, 1, 2, 15, 46};
    static const unsigned period = 10;

    if (n == 0)
        return (seq_t)1;                     /* q_0 = 1 */

    seq_t q_prevprev = (seq_t)0;              /* q_{-1} */
    seq_t q_prev     = (seq_t)1;              /* q_0 */

    for (idx_t i = 1; i <= n; ++i) {
        unsigned ai = a_period[(i - 1) % period];
        seq_t q_curr = (seq_t)ai * q_prev + q_prevprev;
        if (i == n)
            return q_curr;
        q_prevprev = q_prev;
        q_prev = q_curr;
    }
    /* unreachable, but keeps the compiler happy */
    return (seq_t)0;
}
