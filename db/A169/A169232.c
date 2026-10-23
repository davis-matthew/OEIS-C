/*
 * A169232  Number of reduced words of length n in Coxeter group on 27
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^28 = I.
 * https://oeis.org/A169232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For 1 ≤ n ≤ 27: a(n) = 2 + 25 * Σ_{i=0}^{n-1} a(i) For
 *            n ≥ 28: a(n) = 25 * Σ_{i=1}^{27} a(n-i) – 325 * a(n-28) (These
 *            follow from the rational generating function of A169232)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Buffer for the last 28 values. */
    seq_t buf[28] = {0};

    buf[0] = (seq_t)1;
    if (n == 0)
        return buf[0];

    /* Compute a(1) … a(27) using the non‑homogeneous recurrence. */
    seq_t prefix_sum = buf[0];            /* Σ_{i=0}^{k-1} a(i) for current k */
    idx_t k;
    for (k = 1; k <= n && k <= 27; ++k) {
        buf[k] = (seq_t)2 + (seq_t)25 * prefix_sum;
        prefix_sum += buf[k];
    }
    if (n <= 27)
        return buf[n];

    /* Sum of the last 27 values (a(1) … a(27)). */
    seq_t sum27 = 0;
    for (int i = 1; i <= 27; ++i)
        sum27 += buf[i];

    /* Recurrence for n ≥ 28. */
    for (k = 28; k <= n; ++k) {
        /* a(k) = 25*sum27 – 325*a(k-28) */
        seq_t term1 = sum27 * (seq_t)25;
        seq_t term2 = buf[(k - 28) % 28] * (seq_t)325;
        seq_t val   = term1 - term2;

        /* Update circular buffer and sliding window sum. */
        int out_index = (int)(k % 28);           /* index for a(k) */
        int idx_rem   = (int)((k - 27) % 28);   /* element leaving the window */

        sum27 = sum27 - buf[idx_rem] + val;     /* new sum27 for next step */
        buf[out_index] = val;
    }

    return buf[(int)(n % 28)];
}
