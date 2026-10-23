/*
 * A116688  Sum over all partitions of n of the sum of the parts that are
 *          smaller than the largest part.
 * https://oeis.org/A116688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * A000041(n) - M(n), where M(n) = sum_{k=1..n} k *
 *            sum_{m=1}^{⌊n/k⌋} m * p_{<=k-1}(n - m*k) and p_{<=k-1}(t) is the
 *            number of partitions of t using parts ≤ k-1
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Allocate arrays for partition counts (p) and store M(n). */
    seq_t *p = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (p == NULL) return (seq_t)0;

    /* Initialize p[0] = 1, others 0. */
    p[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) p[i] = (seq_t)0;

    seq_t M = (seq_t)0;           /* Sum of (m * k) over all partitions. */

    /* Main loop: iterate over possible largest part k. */
    for (idx_t k = 1; k <= n; ++k) {
        /* Contribution to M from partitions whose largest part is k. */
        for (idx_t m = 1; m * k <= n; ++m) {
            idx_t t = n - m * k;          /* Remaining sum after using m copies of k. */
            seq_t prod = (seq_t)m * (seq_t)k;
            M += prod * p[t];
        }

        /* Update partition counts to include partitions that can use part k. */
        for (idx_t i = k; i <= n; ++i) {
            p[i] += p[i - k];
        }
    }

    /* p[n] now holds the partition number p(n). */
    seq_t result = (seq_t)n * p[n] - M;

    free(p);
    return result;
}
