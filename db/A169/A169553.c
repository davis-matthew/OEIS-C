/*
 * A169553  Number of reduced words of length n in Coxeter group on 12
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^35 = I.
 * https://oeis.org/A169553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A169553: Number of reduced words of length n in a Coxeter group with 12 generators
 * where S_i^2 = I and (S_i S_j)^35 = I for i≠j.
 *
 * The generating function is
 *   G(t) = (t^35 + 2t^34 + … + 2t + 1)/(55t^35 – 10t^34 – … – 10t + 1).
 *
 * From G(t) = Σ a(n)t^n we obtain the linear recurrence
 *
 *   a(n) = c(n) + 10·(a(n‑1)+…+a(n‑34)) – 55·a(n‑35)
 *
 * where
 *
 *   c(0)=c(35)=1,   c(1)…c(34)=2,   c(n)=0 for n>35.
 *
 * The first terms are a(0)=1, a(1)=12, a(2)=132, …
 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* Buffer holding the most recent 35 values:
     * buf[0] = a(k-1), buf[1] = a(k-2), … , buf[34] = a(k-35)  when computing a(k). */
    seq_t buf[35] = {0};
    seq_t ans = 0;

    for (idx_t i = 0; i <= n; ++i) {
        /* Determine c(i) */
        seq_t c;
        if (i == 0 || i == 35) {
            c = 1;
        } else if (i < 35) {
            c = 2;
        } else {
            c = 0;
        }

        /* Sum of the last 34 values a(i-1) … a(i-34) */
        seq_t sum10 = 0;
        for (int j = 0; j < 34; ++j) {
            sum10 += buf[j];
        }

        /* Compute a(i) using the recurrence */
        seq_t val = c + (seq_t)10 * sum10 - (seq_t)55 * buf[34];

        /* Shift buffer right for next iteration */
        for (int k = 34; k >= 1; --k) {
            buf[k] = buf[k - 1];
        }
        buf[0] = val;

        if (i == n) {
            ans = val;
        }
    }

    return ans;
}
