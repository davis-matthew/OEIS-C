/*
 * A164091  Number of reduced words of length n in Coxeter group on 41
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^6 = I.
 * https://oeis.org/A164091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   recurrence from the generating function a(0) = 1 a(1) = 40 a(0)
 *            + 1 for 2 ≤ n ≤ 5: a(n) = 40 a(n-1) a(6) = 40 a(5) - 819 a(0) -
 *            1 a(7) = 40 a(6) - 819 a(1) + 780 a(0) - 1 for n ≥ 8: a(n) = 40
 *            a(n-1) - 819 a(n-6) + 780 a(n-7)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;

    size_t len = (size_t)n + 1;
    seq_t *seq = (seq_t *)malloc(len * sizeof(seq_t));
    if (!seq)
        return (seq_t)0;  /* memory allocation failure */

    seq[0] = (seq_t)1;

    if (n >= 1) {
        seq[1] = (seq_t)40 * seq[0] + (seq_t)1;
    }
    if (n >= 2) {
        idx_t i;
        for (i = 2; i <= 5 && (idx_t)i <= n; ++i) {
            seq[i] = (seq_t)40 * seq[i - 1];
        }
    }
    if (n >= 6) {
        seq[6] = (seq_t)40 * seq[5];
        seq[6] -= (seq_t)819 * seq[0];
        seq[6] -= (seq_t)1;
    }
    if (n >= 7) {
        seq[7] = (seq_t)40 * seq[6];
        seq[7] -= (seq_t)819 * seq[1];
        seq[7] += (seq_t)780 * seq[0];
        seq[7] -= (seq_t)1;
    }
    if (n >= 8) {
        idx_t i;
        for (i = 8; (idx_t)i <= n; ++i) {
            seq[i] = (seq_t)40 * seq[i - 1];
            seq[i] -= (seq_t)819 * seq[i - 6];
            seq[i] += (seq_t)780 * seq[i - 7];
        }
    }

    seq_t result = seq[n];
    free(seq);
    return result;
}
