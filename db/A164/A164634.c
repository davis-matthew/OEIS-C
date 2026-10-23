/*
 * A164634  Number of reduced words of length n in Coxeter group on 21
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^7 = I.
 * https://oeis.org/A164634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence a(n) = 19 a(n-1) + 19
 *            a(n-2) + 19 a(n-3) + 19 a(n-4) + 19 a(n-5) + 19 a(n-6) - 190
 *            a(n-7) together with the initial terms a(0)=1, a(1)=21,
 *            a(2)=21·20, a(3)=21·20², a(4)=21·20³, a(5)=21·20⁴, a(6)=21·20⁵,
 *            a(7)=1,343,999,790
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first seven terms directly */
    if (n == 0) {
        return (seq_t)1;
    }
    if (n == 1) {
        return (seq_t)21;
    }
    /* For 2 ≤ n ≤ 6: a(n) = 21·20^(n‑1) */
    if (n <= 6) {
        seq_t val = (seq_t)21;
        for (idx_t i = 2; i <= n; ++i) {
            val = val * (seq_t)20;
        }
        return val;
    }

    /* Compute the values a(0)…a(7) in a small buffer */
    seq_t buf[8];
    buf[0] = (seq_t)1;          /* a(0) */
    buf[1] = (seq_t)21;         /* a(1) */
    seq_t val = (seq_t)21;
    for (idx_t i = 2; i <= 6; ++i) {  /* a(2)…a(6) */
        val = val * (seq_t)20;
        buf[i] = val;
    }

    /* Sum a(1)…a(6) to compute a(7) */
    seq_t sum_a1_to_a6 = (seq_t)0;
    for (idx_t i = 1; i <= 6; ++i) {
        sum_a1_to_a6 += buf[i];
    }
    /* a(7) = 19 * (sum a(1)…a(6)) - 189 */
    buf[7] = (sum_a1_to_a6 * (seq_t)19) - (seq_t)189;

    /* For n ≥ 8 use the recurrence relation */
    for (idx_t k = 8; k <= n; ++k) {
        idx_t m1 = (k - 1) & 7;
        idx_t m2 = (k - 2) & 7;
        idx_t m3 = (k - 3) & 7;
        idx_t m4 = (k - 4) & 7;
        idx_t m5 = (k - 5) & 7;
        idx_t m6 = (k - 6) & 7;
        idx_t m7 = (k - 7) & 7;

        seq_t next = buf[m1] * (seq_t)19
                   + buf[m2] * (seq_t)19
                   + buf[m3] * (seq_t)19
                   + buf[m4] * (seq_t)19
                   + buf[m5] * (seq_t)19
                   + buf[m6] * (seq_t)19
                   - buf[m7] * (seq_t)190;

        buf[k & 7] = next;
    }

    return buf[n & 7];
}
