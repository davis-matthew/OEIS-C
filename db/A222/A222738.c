/*
 * A222738  Total sum of parts of multiplicity 10 in all partitions of n.
 * https://oeis.org/A222738
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{⌊n/10⌋} k * (p(n-10k) - p(n-11k)) where p(m) is
 *            the partition function (number of partitions of m). The
 *            algorithm builds p(m) incrementally by the Euler pentagonal
 *            recurrence and then evaluates the above sum for each queried n.
 *            It uses only integer arithmetic and works for all pro...
 * Range:     u8 n <= 31, u16 n <= 60, u32 n <= 145, u64 n <= 429,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Global cache for partition numbers */
static seq_t *partitions = NULL;
static idx_t   partition_cap = 0;   /* highest computed index, inclusive */

/* Build partition numbers p[0..N] using Euler's recurrence.
   Only increases the cache if necessary. */
static void ensure_partitions(idx_t N) {
    if (partition_cap >= N) return;          /* already enough */

    /* Expand the cache */
    seq_t *new_part = (seq_t *)realloc(partitions,
                                       (N + 1) * sizeof(seq_t));
    if (!new_part) return;                   /* silently fail */
    partitions = new_part;

    if (partition_cap == 0) {
        /* First allocation: initialise all to 0, set p[0] = 1 */
        for (idx_t i = 0; i <= N; ++i) partitions[i] = 0;
        partitions[0] = 1;
    } else {
        /* New entries must start at 0 */
        for (idx_t i = partition_cap + 1; i <= N; ++i) partitions[i] = 0;
    }

    /* Compute new partition values */
    for (idx_t i = partition_cap + 1; i <= N; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1;; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > i) break;
            if (k & 1)
                sum += partitions[i - pent1];
            else
                sum -= partitions[i - pent1];

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 > i) continue;
            if (k & 1)
                sum += partitions[i - pent2];
            else
                sum -= partitions[i - pent2];
        }
        partitions[i] = sum;
    }

    partition_cap = N;
}

/* Main function required by the problem statement */
seq_t a(idx_t n) {
    if (n < 10) return 0;        /* undefined below the offset */

    ensure_partitions(n);        /* need p[0..n] */

    seq_t total = 0;
    idx_t max_k = n / 10;        /* largest part size that can appear 10 times */

    for (idx_t k = 1; k <= max_k; ++k) {
        idx_t n1 = n - 10 * k;   /* n - 10k is always ≥ 0 here */
        seq_t term = partitions[n1];

        /* Subtract partitions of n-11k only if the index is valid */
        if (n >= 11 * k) {
            idx_t n2 = n - 11 * k;
            term -= partitions[n2];
        }

        total += term * (seq_t)k; /* multiply by part size k */
    }

    return total;
}
