/*
 * A209122  Numbers a(n) for which there exists k>1 such that the number of
 *          partitions of a(n) into k parts is k.
 * https://oeis.org/A209122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + p(n), for n > 2 where p(n) is the partition number
 *            A000041(n). For n = 1,2, a(1) = 4, a(2) = 5. Uses Euler's
 *            pentagonal number recurrence
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *partition_table = NULL;   /* cached partition numbers */
static idx_t partition_size = 0;        /* max n computed so far */

/* Compute partition numbers up to n using Euler's recurrence. */
static void compute_partitions(idx_t n)
{
    if (n <= partition_size) return;   /* already computed */

    /* allocate new table large enough */
    idx_t new_size = n;
    seq_t *new_table = (seq_t*)malloc((new_size + 1) * sizeof(seq_t));
    if (!new_table) { /* allocation failed – let caller handle error */ return; }

    /* copy existing data if any */
    if (partition_table) {
        memcpy(new_table, partition_table, (partition_size + 1) * sizeof(seq_t));
        free(partition_table);
    }

    /* initialize p(0) if this is the first allocation */
    if (partition_size == 0) {
        new_table[0] = (seq_t)1;            /* p(0) = 1 */
    }

    /* compute new partition numbers */
    for (idx_t i = (partition_size == 0 ? 1 : partition_size + 1); i <= new_size; ++i) {
        seq_t pos = 0, neg = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > i) break;
            if (k & 1) pos += new_table[i - pent1];
            else       neg += new_table[i - pent1];

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 > i) continue;
            if (k & 1) pos += new_table[i - pent2];
            else       neg += new_table[i - pent2];
        }
        new_table[i] = pos - neg;  /* pos >= neg for partitions */
    }

    partition_table = new_table;
    partition_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)5;

    compute_partitions(n);   /* ensure p(n) is available */

    return (seq_t)n + partition_table[n];
}
