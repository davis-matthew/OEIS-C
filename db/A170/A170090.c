/*
 * A170090  Number of reduced words of length n in Coxeter group on 33
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^37 = I.
 * https://oeis.org/A170090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   The sequence has generating function * N(t)/D(t) with * N(t) = 1
 *            + 2t + 2t² + … + 2t³⁶ + t³⁷ * D(t) = 1 – 31t – 31t² – … – 31t³⁶
 *            + 496t³⁷. * * From this we obtain the linear recurrence * a(0) =
 *            1 * for 1 ≤ n ≤ 36 : * a(n) = 31 · (a(0)+…+a(n‑1)) + 2 * for n ≥
 *            37 : * a(n) = 31 · (a(n‑1)+…+a(n‑36))...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* Circular buffer for the last 37 values. */
    seq_t buf[37];
    buf[0] = 1;
    /* Sum of the values currently in buf that are needed
       for the next recurrence step. Initially just a(0). */
    seq_t sum_last36 = 1;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t curr;
        if (i <= 36) {
            /* a(i) = 31 * sum_of_all_previous + 2 */
            curr = ((seq_t)31) * sum_last36 + ((seq_t)2);
        } else {
            /* Remove a(i-37) from the window of the last 36 terms. */
            seq_t prev37 = buf[(i - 37) % 37];
            sum_last36 -= prev37;

            /* a(i) = 31 * sum_of_last36 - 496 * a(i-37) */
            curr = ((seq_t)31) * sum_last36 - ((seq_t)496) * prev37;
        }

        /* Store the new value in the circular buffer */
        buf[i % 37] = curr;

        /* Update window sum for the next step */
        sum_last36 += curr;
    }

    return buf[n % 37];
}
