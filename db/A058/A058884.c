/*
 * A058884  Partial sums of the partition function (A000041), with the last
 *          term subtracted. Also the sum of the row of the character table
 *          for S_n corresponding to the partition n-1,1 for n>1. Also the sum
 *          over all partitions lambda of n of one less than the number of 1's
 *          in lambda.
 * https://oeis.org/A058884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = A000070(n-1) - A000041(n) for n > 0 (with a(0) = -1).
 *            A000070(k) is the partial sum of the partition numbers, and
 *            A000041(k) is the partition function p(k). This implementation
 *            directly computes p(k) for 0 ≤ k ≤ n and the cumulative sum
 *            S(n-1); it then returns S(n-1) - p(n)
 * Range:     i8 n <= 12, i16 n <= 32, i32 n <= 104, i64 n <= 363,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)-1;               /* a(0) = -1 */
    }

    /* Allocate array to hold partition numbers p[0..n] */
    seq_t *p = (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!p) {
        return (seq_t)-1;               /* allocation failure (should not occur) */
    }
    p[0] = (seq_t)1;                   /* p(0) = 1 */

    /* Compute p(k) for k = 1..n using Euler's pentagonal recurrence */
    for (idx_t m = 1; m <= n; ++m) {
        seq_t s = (seq_t)0;
        for (idx_t k = 1; ; ++k) {
            idx_t g1 = k * ((idx_t)3 * k - 1) / 2;
            if (g1 > m) break;
            seq_t sign = (k & 1) ? (seq_t)1 : (seq_t)-1;
            s += sign * p[m - g1];

            idx_t g2 = k * ((idx_t)3 * k + 1) / 2;
            if (g2 <= m) {
                s += sign * p[m - g2];
            }
        }
        p[m] = s;
    }

    /* Compute the partial sum S(n-1) = sum_{i=0}^{n-1} p(i) */
    seq_t cum = (seq_t)0;
    for (idx_t m = 0; m <= n - 1; ++m) {
        cum += p[m];
    }

    seq_t result = cum - p[n];

    free(p);
    return result;
}
