/*
 * A222706  Total number of parts of multiplicity 6 in all partitions of n.
 * https://oeis.org/A222706
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \sum_{k \ge 1} p(n-6k) - \sum_{k \ge 1} p(n-7k) where
 *            p(m) is the partition number of m
 * Range:     u8 n <= 26, u16 n <= 55, u32 n <= 142, u64 n <= 432,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* For n below the offset, return 0. */
    if (n < 6) {
        return (seq_t)0;
    }

    /* Compute partition numbers p[0..n] using the generalized pentagonal theorem. */
    size_t len = (size_t)n + 1;
    seq_t *p = (seq_t *)calloc(len, sizeof(seq_t));
    if (!p) return (seq_t)0;            /* malloc failure guard */
    p[0] = (seq_t)1;

    for (size_t i = 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (size_t k = 1; ; ++k) {
            size_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > i) break;
            if ((k & 1) == 1)
                sum += p[i - pent1];
            else
                sum -= p[i - pent1];

            size_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 > i) continue;
            if ((k & 1) == 1)
                sum += p[i - pent2];
            else
                sum -= p[i - pent2];
        }
        p[i] = sum;
    }

    /* Compute the required difference of partition sums. */
    seq_t total = (seq_t)0;
    size_t lim6 = n / 6;
    for (size_t k = 1; k <= lim6; ++k) {
        total += p[n - 6 * k];
    }
    size_t lim7 = n / 7;
    for (size_t k = 1; k <= lim7; ++k) {
        total -= p[n - 7 * k];
    }

    free(p);
    return total;
}
