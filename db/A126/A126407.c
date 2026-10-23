/*
 * A126407  Number of base 20 n-digit numbers with adjacent digits differing
 *          by two or less.
 * https://oeis.org/A126407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of base‑20 n‑digit numbers whose adjacent digits
 *            differ by at most 2. The sequence is computed by a simple
 *            dynamic programming recurrence on the last digit
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) {
        return (seq_t)1;
    }

    /* DP arrays for the previous and current iteration (digits 0–19). */
    seq_t f_prev[20];
    seq_t f_cur[20];

    /* Base case n = 1: every digit 0–19 is allowed. */
    for (int d = 0; d < 20; ++d) {
        f_prev[d] = (seq_t)1;
    }
    if (n == 1) {
        return (seq_t)20;
    }

    seq_t total = (seq_t)20;  /* value for n = 1 */

    for (idx_t cur = 2; cur <= n; ++cur) {
        /* Compute f_cur[d] = sum_{e: |e-d|<=2} f_prev[e] */
        for (int d = 0; d < 20; ++d) {
            seq_t sum = (seq_t)0;
            int lo = d - 2;
            if (lo < 0) lo = 0;
            int hi = d + 2;
            if (hi > 19) hi = 19;
            for (int e = lo; e <= hi; ++e) {
                sum += f_prev[e];
            }
            f_cur[d] = sum;
        }

        /* Sum over all last digits to get a(cur) */
        total = (seq_t)0;
        for (int d = 0; d < 20; ++d) {
            total += f_cur[d];
        }

        /* Prepare for next iteration */
        for (int d = 0; d < 20; ++d) {
            f_prev[d] = f_cur[d];
        }
    }

    return total;
}
