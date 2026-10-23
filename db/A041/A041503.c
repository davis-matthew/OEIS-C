/*
 * A041503  Denominators of continued fraction convergents to sqrt(268).
 * https://oeis.org/A041503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the nth convergent of the continued
 *            fraction for √268. The period of the continued fraction (after
 *            the initial term) is 20, with partial quotients:
 *            2,1,2,3,3,1,3,1,10,8,10,1,3,1,3,3,2,1,2,32. The recurrence for
 *            the denominators is q_{-2}=1, q_{-1}=0, and for k≥0: q_k = a_k
 *            *...
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Periodic part of the continued fraction of √268 (after a0).  */
    static const seq_t period[20] = {
        2, 1, 2, 3, 3, 1, 3, 1, 10, 8,
        10, 1, 3, 1, 3, 3, 2, 1, 2, 32
    };

    seq_t q_prev2 = (seq_t)1;   /* q_{-2} */
    seq_t q_prev1 = (seq_t)0;   /* q_{-1} */
    seq_t q      = (seq_t)0;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t ak;
        if (k == 0) {
            ak = (seq_t)16;          /* a0 = ⌊√268⌋ */
        } else {
            ak = period[(k - 1) % 20];
        }

        q = ak * q_prev1 + q_prev2;   /* recurrence */
        q_prev2 = q_prev1;
        q_prev1 = q;
    }
    return q;
}
