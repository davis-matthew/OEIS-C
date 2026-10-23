/*
 * A170476  Number of reduced words of length n in Coxeter group on 35
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^45 = I.
 * https://oeis.org/A170476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = 2 + 33 * (a(0)+a(1)+...+a(n-1)) for 1 ≤ n ≤ 44
 *            a(n) = 33 * (a(n-1)+a(n-2)+...+a(n-44)) − 561 * a(n-45) for n ≥
 *            45 (These recurrence relations are derived from the rational
 *            generating function given in the OEIS entry.)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    const idx_t K = 45;                       /* size of the sliding buffer */
    seq_t buf[K];                             /* circular buffer for last 45 values */

    /* a(0) = 1 */
    buf[0] = (seq_t)1;
    if (n == 0) return buf[0];

    /* Compute values a(1) … a(44) using the prefix‑sum recurrence. */
    seq_t prefix = buf[0];                    /* sum of a(0) … a(i-1) */
    idx_t max_small = (n < 44 ? n : 44);
    for (idx_t i = 1; i <= max_small; ++i) {
        seq_t val = (seq_t)2 + (seq_t)33 * prefix;
        buf[i] = val;
        prefix += val;
    }
    if (n <= 44) return buf[n];

    /* Now a(0)…a(44) are known.  Prepare for the main recurrence. */
    seq_t sum_last44 = prefix - buf[0];      /* sum of a(1) … a(44) */

    for (idx_t i = 45; i <= n; ++i) {
        seq_t term_45_ago = buf[(i - 45) % K];      /* a(i-45) */
        seq_t new_val = (seq_t)33 * sum_last44
                        - (seq_t)561 * term_45_ago;

        seq_t term_to_remove = buf[(i - 44) % K];    /* a(i-44) */
        sum_last44 += new_val;
        sum_last44 -= term_to_remove;

        buf[i % K] = new_val;
    }

    return buf[n % K];
}
