/*
 * A222710  Total number of parts of multiplicity 10 in all partitions of n.
 * https://oeis.org/A222710
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{m≥1} p(n-10m) – Σ_{m≥1} p(n-11m)
 * Range:     u8 n <= 32, u16 n <= 62, u32 n <= 152, u64 n <= 448,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *partition = NULL;
static idx_t part_cap = 0;

static void compute_partitions_upto(idx_t idx)
{
    if (idx < part_cap)
        return;

    idx_t new_cap = idx + 1;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr)
        exit(1);

    for (idx_t i = 0; i < part_cap; ++i)
        new_arr[i] = partition[i];

    if (new_cap > 0)
        new_arr[0] = (seq_t)1;

    for (idx_t n = (part_cap > 0 ? part_cap : 1); n < new_cap; ++n) {
        seq_t pos = 0, neg = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > n) break;
            if (k & 1U)
                pos += new_arr[n - g1];
            else
                neg += new_arr[n - g1];
            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 > n) continue;
            if (k & 1U)
                pos += new_arr[n - g2];
            else
                neg += new_arr[n - g2];
        }
        new_arr[n] = pos - neg;
    }

    free(partition);
    partition = new_arr;
    part_cap = new_cap;
}

seq_t a(idx_t n)
{
    compute_partitions_upto(n);
    seq_t pos_sum = 0, neg_sum = 0;
    for (idx_t m = 1; m * 10 <= n; ++m)
        pos_sum += partition[n - m * 10];
    for (idx_t m = 1; m * 11 <= n; ++m)
        neg_sum += partition[n - m * 11];
    return pos_sum - neg_sum;
}
