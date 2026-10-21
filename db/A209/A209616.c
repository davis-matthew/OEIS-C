/*
 * A209616  Sum of positive Dyson's ranks of all partitions of n.
 * https://oeis.org/A209616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in (1/∏_{k≥1}(1−x^k)) * *
 *            Σ_{k≥1}(−1)^{k−1}·x^{k(3k+1)/2}/(1−x^k)
 * Range:     u8 n <= 14, u16 n <= 36, u32 n <= 112, u64 n <= 378,
 *            u128 n <= 1368, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term (offset 1) of OEIS A209616.
 *
 * The algorithm uses the generating function formula.
 * First it generates the partition numbers p[0..n] by the
 * pentagonal‑number recurrence. Then, for each integer k with
 * g_k = k(3k+1)/2 ≤ n, it adds or subtracts the corresponding
 * partition numbers p[n−(g_k + k·m)] to a running positive and
 * negative sum.  Finally a(n) = pos_sum – neg_sum.  This works
 * for all supported seq_t types because only addition and
 * subtraction of non‑negative values are used.
 *
 * Time complexity:   O(n·√n)
 * Memory usage:      O(n)
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Special case: the sequence is defined for n ≥ 1.
     * For n = 0 we return 0 (unused but harmless). */
    if (n == 0) return (seq_t)0;

    /* Allocate array for partition numbers p[0..n] */
    seq_t *p = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!p) return (seq_t)0;   /* allocation failure – unlikely in tests */

    /* Base case: p(0) = 1 */
    p[0] = (seq_t)1;

    /* Compute partition numbers using the pentagonal number theorem */
    for (idx_t m = 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; ; ++k) {
            idx_t g1 = k * (3 * k - 1) / 2;   /* generalized pentagonal k(3k-1)/2 */
            idx_t g2 = k * (3 * k + 1) / 2;   /* generalized pentagonal k(3k+1)/2 */
            if (g1 > m && g2 > m) break;

            if (g1 <= m) {
                sum += (k & 1 ? 1 : -1) * p[m - g1];
            }
            if (g2 <= m) {
                sum += (k & 1 ? 1 : -1) * p[m - g2];
            }
        }
        p[m] = sum;
    }

    /* Accumulate positive and negative contributions separately */
    seq_t pos = (seq_t)0;
    seq_t neg = (seq_t)0;

    /* Determine maximal k with g_k' ≤ n */
    idx_t kmax = 0;
    for (idx_t k = 1;; ++k) {
        idx_t gk = k * (3 * k + 1) / 2;
        if (gk > n) { kmax = k - 1; break; }
    }

    for (idx_t k = 1; k <= kmax; ++k) {
        idx_t gk = k * (3 * k + 1) / 2;
        int sign = (k & 1) ? 1 : -1;   /* (-1)^{k-1} */

        for (idx_t m = 0;; ++m) {
            idx_t exp = gk + k * m;
            if (exp > n) break;
            seq_t val = p[n - exp];
            if (sign > 0) pos += val; else neg += val;
        }
    }

    /* Free temporary array */
    free(p);

    /* Final result: pos - neg  (pos ≥ neg) */
    seq_t result = pos - neg;
    return result;
}
