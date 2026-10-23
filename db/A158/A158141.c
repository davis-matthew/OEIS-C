/*
 * A158141  Number of nondecreasing integer sequences of length 7 with sum
 *          zero and sum of absolute values 2n.
 * https://oeis.org/A158141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{p=1}^{7} Σ_{q=1}^{7-p} P(n,p)*P(n,q) where P(n,k) is
 *            the number of integer partitions of n into exactly k parts
 * Range:     u8 n <= 8, u16 n <= 30, u32 n <= 305, u64 n <= 787,
 *            u128 n <= 787, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* --------- Local static data for the partition table ---------- */
static seq_t *partition_table = NULL;      /* 1‑dim array: (n+1)*(7+1) */
static idx_t partition_nmax = 0;           /* largest n for which table is filled */
static const idx_t PART_MAX = 7;           /* maximum number of parts needed */
static const size_t ROWS = PART_MAX + 1;   /* columns for k=0..7 */

/* ---------- Ensure the table contains values up to n ---------- */
static void ensure_partitions(idx_t n)
{
    if (n <= partition_nmax) return;

    idx_t new_n = n;
    size_t new_size = (size_t)(new_n + 1) * ROWS;

    seq_t *new_table;
    if (partition_table == NULL) {
        new_table = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_table) return;
        /* initialise all values to zero */
        for (size_t i = 0; i < new_size; ++i) new_table[i] = 0;
        /* base case: P(0,0) = 1 */
        new_table[0] = 1;
    } else {
        size_t old_size = (size_t)(partition_nmax + 1) * ROWS;
        new_table = (seq_t *)realloc(partition_table, new_size * sizeof(seq_t));
        if (!new_table) return;
        /* zero the new part of the array */
        for (size_t i = old_size; i < new_size; ++i) new_table[i] = 0;
    }

    /* Compute values for n = partition_nmax+1 .. new_n */
    for (idx_t cur = partition_nmax + 1; cur <= new_n; ++cur) {
        size_t base_cur = (size_t)cur * ROWS;
        for (idx_t k = 1; k <= PART_MAX; ++k) {
            seq_t val = new_table[(size_t)(cur - 1) * ROWS + (k - 1)];
            if (cur >= k) val += new_table[(size_t)(cur - k) * ROWS + k];
            new_table[base_cur + k] = val;
        }
        /* new_table[base_cur + 0] remains 0 for cur > 0 */
    }

    partition_table = new_table;
    partition_nmax   = new_n;
}

/* --------- Main sequence function ------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    ensure_partitions(n);

    seq_t sum = 0;
    size_t base_n = (size_t)n * ROWS;

    for (idx_t p = 1; p <= PART_MAX; ++p) {
        seq_t cp = partition_table[base_n + p];
        if (cp == 0) continue;
        for (idx_t q = 1; q <= PART_MAX - p; ++q) {
            seq_t cq = partition_table[base_n + q];
            if (cq == 0) continue;
            sum = sum + cp * cq;
        }
    }
    return sum;
}
