/*
 * A041993  Denominators of continued fraction convergents to sqrt(519).
 * https://oeis.org/A041993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) is the denominator of the n‑th convergent of √519.
 *            Recurrence: Let b_{-2}=1, b_{-1}=0, a₀=22. The continued
 *            fraction expansion of √519 is [22;
 *            overline{1,3,1,1,2,1,2,3,7,3,2,1,2,1,1,3,1,44}]. For k>0 let a_k
 *            be the k‑th element of this repeating period (so a_k =
 *            period[(k-1) mod 18]). Then b...
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 47, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Periodic part of the continued fraction after the initial term 22. */
    static const unsigned int period[] = {
        1, 3, 1, 1, 2, 1, 2, 3, 7, 3, 2, 1, 2, 1, 1, 3, 1, 44
    };
    const unsigned int period_len = sizeof(period) / sizeof(period[0]); /* 18 */

    seq_t q_prev_prev = (seq_t)1; /* b_{-2} */
    seq_t q_prev      = (seq_t)0; /* b_{-1} */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t ak;
        if (k == 0) {
            ak = (seq_t)22;
        } else {
            unsigned int idx = (unsigned int)((k - 1) % period_len);
            ak = (seq_t)period[idx];
        }
        seq_t q = ak * q_prev + q_prev_prev;
        q_prev_prev = q_prev;
        q_prev = q;
    }
    return q_prev;
}
