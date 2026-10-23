/*
 * A042819  Denominators of continued fraction convergents to sqrt(940).
 * https://oeis.org/A042819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 21, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of continued fraction convergents to sqrt(940).
   Let a(0)=30 be the integer part of sqrt(940).
   For k ≥ 0 the partial quotients repeat the pattern
   30, 1, 1, 1, 14, 1, 1, 1, 60, 1, 1, 1, 14, ….
   With p(-2)=0, p(-1)=1, q(-2)=1, q(-1)=0 the convergents satisfy
   p(k)=a(k)p(k-1)+p(k-2),  q(k)=a(k)q(k-1)+q(k-2).
   a(n)=q(n).  The first few terms are 1, 1, 2, 3, 44, … */

seq_t a(idx_t n)
{
    /* partial quotients for the repeating period of sqrt(940) */
    static const seq_t period[] = {1, 1, 1, 14, 1, 1, 1, 60};
    static const idx_t period_len = 8;

    seq_t p_prev2 = (seq_t)0;
    seq_t p_prev1 = (seq_t)1;
    seq_t q_prev2 = (seq_t)1;
    seq_t q_prev1 = (seq_t)0;
    seq_t p_curr, q_curr;
    seq_t ak;

    for (idx_t k = 0; k <= n; ++k) {
        if (k == 0) {
            ak = (seq_t)30;                /* integer part of sqrt(940) */
        } else {
            ak = period[(k - 1) % period_len];
        }
        p_curr = ak * p_prev1 + p_prev2;
        q_curr = ak * q_prev1 + q_prev2;
        /* advance the indices */
        p_prev2 = p_prev1;
        p_prev1 = p_curr;
        q_prev2 = q_prev1;
        q_prev1 = q_curr;
    }
    return q_prev1;
}
