/*
 * A158177  Number of nondecreasing integer sequences of length 43 with sum
 *          zero and sum of absolute values 2n.
 * https://oeis.org/A158177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{min(n,43)} sum_{m=1}^{min(43-k,n)} P_k(n) *
 *            P_m(n), where P_k(n) is the number of integer partitions of n
 *            into exactly k parts (nondecreasing sequence of length k with
 *            positive parts)
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 43, u64 n <= 134, u128 n <= 199,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

#define KMAX 43

/* allocate space for partition table p[k][s], k = 0..KMAX, s = 0..n */
static seq_t *partition_table = NULL;        /* compact 1‑D array */
static size_t partition_cols   = 0;          /* n+1 of current table */

/* enlarge the partition table to support all s <= n_needed */
static void
ensure_partition_table(size_t n_needed)
{
    if (partition_table != NULL && partition_cols > n_needed) return;

    /* free old table if any */
    free(partition_table);

    partition_cols = n_needed + 1;
    size_t total = (KMAX + 1) * partition_cols;
    partition_table = (seq_t *)malloc(total * sizeof(seq_t));
    if (partition_table == NULL) {
        exit(1); /* out of memory */
    }

    /* row 0 : only one way to partition 0 with 0 parts */
    for (size_t s = 0; s < partition_cols; ++s) {
        partition_table[s] = (s == 0 ? (seq_t)1 : (seq_t)0);
    }

    /* DP: P_k(n) for k >= 1 */
    for (int k = 1; k <= KMAX; ++k) {
        for (size_t s = 0; s < partition_cols; ++s) {
            seq_t val = 0;
            if (s >= (size_t)k) {
                val += partition_table[(k) * partition_cols + (s - k)];
            }
            if (s >= 1) {
                val += partition_table[(k - 1) * partition_cols + (s - 1)];
            }
            partition_table[k * partition_cols + s] = val;
        }
    }
}

/* number of partitions of n into exactly k parts (k <= KMAX) */
static inline seq_t
partitions_exact(int k, size_t n)
{
    if (k < 0 || k > KMAX) return 0;
    return partition_table[k * partition_cols + n];
}

/* main sequence function */
seq_t
a(idx_t n)
{
    if (n == (idx_t)0) {
        /* sum of absolute values 0 -> all zeros */
        return (seq_t)1;
    }

    /* ensure tables large enough for this n */
    size_t usz = (size_t)n;
    if (partition_cols <= usz) {
        ensure_partition_table(usz);
    }

    size_t maxk = (usz < KMAX) ? usz : KMAX;
    seq_t sum = 0;

    for (int k = 1; k <= (int)maxk; ++k) {
        seq_t pk = partitions_exact(k, usz);
        if (pk == 0) continue;

        int maxm = 43 - k;
        if (maxm > (int)usz) maxm = (int)usz;
        for (int m = 1; m <= maxm; ++m) {
            seq_t pm = partitions_exact(m, usz);
            if (pm == 0) continue;
            sum += pk * pm;
        }
    }
    return sum;
}
