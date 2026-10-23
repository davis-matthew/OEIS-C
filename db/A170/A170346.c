/*
 * A170346  Number of reduced words of length n in Coxeter group on 49
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^42 = I.
 * https://oeis.org/A170346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) follows the linear recurrence a(n) = 47 *
 *            (a(n-1)+...+a(n-41)) - 1128 * a(n-42) for n ≥ 42 with the first
 *            42 values given by a(0) = 1, a(n) = 2 + 47 * (a(0)+...+a(n-1))
 *            for 1 ≤ n ≤ 41. This implements the recurrence derived from the
 *            generating function of A170346
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* buffer holds the last 42 computed terms */
    seq_t buf[42] = {0};
    seq_t sum_all = 0;   /* sum of all terms seen so far (for n < 42)          */
    seq_t sum41   = 0;   /* sum of the last 41 terms (used when n ≥ 42)       */
    seq_t val;

    for (idx_t i = 0; i <= n; ++i) {
        if (i == 0) {                    /* a(0) */
            val = (seq_t)1;
            buf[0] = val;
            sum_all = val;
            continue;
        }

        if (i <= 41) {                   /* 1 ≤ i ≤ 41: use the initial recurrence   */
            val = (seq_t)2 + (seq_t)47 * sum_all;
            buf[i % 42] = val;
            sum_all += val;
        } else { /* i ≥ 42: use the homogeneous recurrence */
            if (i == 42) {
                /* sum of a(1)..a(41) = sum_all - a(0) */
                sum41 = sum_all - buf[0];
            } else {
                /* update sliding window for the last 41 terms */
                sum41 += buf[(i - 1) % 42];
                sum41 -= buf[(i - 42) % 42];
            }
            val = (seq_t)47 * sum41 - (seq_t)1128 * buf[(i - 42) % 42];
            buf[i % 42] = val;
        }
    }

    return buf[n % 42];
}
