/*
 * A041740  Numerators of continued fraction convergents to sqrt(390).
 * https://oeis.org/A041740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = a(n-1) * ck + a(n-2), n ≥ 2 where ck = 1, 2, 1, 38
 *            repeated (ck = a_k for k≥1), a(0) = 19, a(1) = 20. (These are
 *            the numerators of the continued‑fraction convergents of
 *            sqrt(390).)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 9‑term recurrence: p_{-2}=0, p_{-1}=1, a_0=19, a_{k>0} periodic as below. */
    static const int period[4] = {1, 2, 1, 38};  /* a_1, a_2, a_3, a_4 */
    seq_t p_prev2 = 0;   /* p_{k-2} */
    seq_t p_prev1 = 1;   /* p_{k-1} */
    seq_t p_curr;

    for (idx_t k = 0; k <= n; ++k) {
        int a_k = (k == 0) ? 19 : period[(k - 1) % 4];
        p_curr = (seq_t)a_k * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = p_curr;
    }
    return p_prev1;   /* p_n = a(n) */
}
