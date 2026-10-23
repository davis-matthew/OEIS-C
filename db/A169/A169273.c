/*
 * A169273  Number of reduced words of length n in Coxeter group on 20
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^29 = I.
 * https://oeis.org/A169273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is given by the rational generating function G(t) = (t^29 +
 *            2t^28 + ... + 2t + 1) / (171t^29 - 18t^28 - 18t^27 - … - 18t +
 *            1). This yields the linear recurrence a(0) = 1, a(n) = 2 for
 *            1≤n≤28 + 18·∑_{k=0}^{n-1}a(k), and for n ≥ 29 a(n) =
 *            18·(a(n-1)+…+a(n-28)) – 171·a(n-29)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Buffer for the last 29 terms, cyclically used. */
    static seq_t buf[29];
    seq_t prefix_sum = 0; /* sum of a[0] … a[i] while i≤28 */

    /* Handle the very first term specially. */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Compute terms up to n or 28 (whichever is smaller). */
    buf[0] = (seq_t)1;
    prefix_sum = buf[0];

    if (n <= 28) {
        for (idx_t i = 1; i <= n; ++i) {
            buf[i] = (seq_t)2 + (seq_t)18 * prefix_sum;
            prefix_sum += buf[i];
        }
        return buf[n];
    }

    /* Fill 1..28 */
    for (idx_t i = 1; i <= 28; ++i) {
        buf[i] = (seq_t)2 + (seq_t)18 * prefix_sum;
        prefix_sum += buf[i];
    }

    /* window_sum = sum of the last 28 terms a[1]…a[28] */
    seq_t window_sum = prefix_sum - buf[0];
    seq_t ai = 0;

    for (idx_t i = 29; i <= n; ++i) {
        /* a(i) = 18·window_sum – 171·a(i‑29) */
        ai = (seq_t)18 * window_sum - (seq_t)171 * buf[i - 29];

        /* Update the moving sum for the next step: */
        window_sum += ai;            /* add new term */
        window_sum -= buf[i - 28];   /* remove term that leaves window */

        /* Store the new term in the cyclic buffer */
        buf[i % 29] = ai;
    }

    return ai;
}
