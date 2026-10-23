/*
 * A025158  Number of partitions of n with distinct parts p(i) such that if i
 *          != j, then |p(i) - p(j)| >= 4.
 * https://oeis.org/A025158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{m>=1, 2*m*(m-1)<=n} ( P[n-2*m*(m-1)][m] -
 *            P[n-2*m*(m-1)][m-1] ) where P[s][k] counts partitions of s whose
 *            largest part is <= k (equivalently partitions of s into at most
 *            k parts)
 * Range:     u8 n <= 48, u16 n <= 129, u32 n <= 390, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *partition_table = NULL;   /* 1‑D array storing P[n][k] */
static idx_t    table_n = 0;            /* current maximum n stored */
static idx_t    table_m = 0;            /* current maximum m stored */
static idx_t    table_w = 0;            /* width = table_m + 1          */

/* ensure that partition_table holds P[s][k] for 0 ≤ s ≤ n, 0 ≤ k ≤ m_needed */
static void ensure_dp(idx_t n)
{
    /* compute maximal m needed for this n: 2*m*(m-1) <= n  */
    idx_t m_needed = 0;
    while (2 * (m_needed + 1) * m_needed <= n)
        ++m_needed;   /* after loop m_needed is maximal satisfying the inequality */

    if (n <= table_n && m_needed <= table_m)
        return;              /* already sufficient */

    /* reallocate new table */
    idx_t new_n = n;
    idx_t new_m = m_needed;
    idx_t width   = new_m + 1;
    size_t size   = (size_t)(new_n + 1) * width;

    seq_t *new_tbl = (seq_t *)malloc(size * sizeof(seq_t));
    if (!new_tbl)  /* memory allocation failed – abort */
        abort();

    /* P[0][k] = 1 for all k */
    for (idx_t k = 0; k <= new_m; ++k)
        new_tbl[0 * width + k] = 1;

    /* DP recurrence:
       P[s][k] = P[s][k-1] + (s>=k ? P[s-k][k] : 0)   for s>0, k>0
       P[s][0] = 0   for s>0
    */
    for (idx_t s = 1; s <= new_n; ++s) {
        new_tbl[s * width + 0] = 0;
        for (idx_t k = 1; k <= new_m; ++k) {
            seq_t val1 = new_tbl[s * width + (k - 1)];
            seq_t val2 = (s >= k) ? new_tbl[(s - k) * width + k] : (seq_t)0;
            new_tbl[s * width + k] = val1 + val2;
        }
    }

    /* free old table and update globals */
    if (partition_table)
        free(partition_table);
    partition_table = new_tbl;
    table_n = new_n;
    table_m = new_m;
    table_w = width;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n<1, but safe */

    ensure_dp(n);                     /* build necessary table */
    const idx_t width = table_w;      /* width = table_m + 1 */

    seq_t result = 0;
    for (idx_t m = 1; 2 * m * (m - 1) <= n; ++m) {
        idx_t s = n - 2 * m * (m - 1);                /* S = n - 2*m*(m-1) */
        size_t base = (size_t)s * width;
        seq_t term = partition_table[base + m] - partition_table[base + (m - 1)];
        result += term;
    }
    return result;
}
