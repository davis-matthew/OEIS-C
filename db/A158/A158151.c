/*
 * A158151  Number of nondecreasing integer sequences of length 17 with sum
 *          zero and sum of absolute values 2n.
 * https://oeis.org/A158151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{16} Σ_{l=1}^{16} p(n,k)·p(n,l) for k+l≤17, where
 *            p(n,k) is the number of partitions of n into exactly k positive
 *            parts. This follows directly from the characterization of the
 *            required nondecreasing sequences
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 51, u64 n <= 282, u128 n <= 363,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#define MAX_K 16

/* Static table of partition numbers p(n,k) for 0 ≤ n ≤ current limit,
   0 ≤ k ≤ MAX_K.  */
static idx_t table_max_n = 0;
static seq_t *table_data   = NULL;

static inline size_t idx_of(idx_t n, idx_t k)
{
    return (size_t)n * (MAX_K + 1) + (size_t)k;
}

/* Ensure that table_data contains rows 0..n (inclusive).  */
static void ensure_table(idx_t n)
{
    if (table_data == NULL)
    {
        table_data = (seq_t *)malloc((MAX_K + 1) * sizeof(seq_t));
        if (!table_data) return;
        /* initialise row 0 : p(0,0)=1, p(0,k>0)=0 */
        for (idx_t k = 0; k <= MAX_K; ++k)
            table_data[idx_of(0, k)] = (k == 0) ? (seq_t)1 : (seq_t)0;
        table_max_n = 0;
    }

    if (n <= table_max_n) return;

    /* extend the array */
    size_t new_rows = (size_t)(n + 1);
    size_t new_size = new_rows * (MAX_K + 1);
    seq_t *new_data = (seq_t *)realloc(table_data, new_size * sizeof(seq_t));
    if (!new_data) return;            /* allocation failure – cannot extend */
    table_data = new_data;

    /* compute new rows */
    for (idx_t nn = table_max_n + 1; nn <= n; ++nn)
    {
        for (idx_t k = 0; k <= MAX_K; ++k)
        {
            if (k == 0)
            {
                table_data[idx_of(nn, 0)] = (seq_t)0;   /* n>0 */
            }
            else
            {
                seq_t val = 0;
                if (nn >= k)
                {
                    val = table_data[idx_of(nn - 1, k - 1)] +
                          table_data[idx_of(nn - k, k)];
                }
                table_data[idx_of(nn, k)] = val;
            }
        }
    }

    table_max_n = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* sequence is defined for n ≥ 1 */

    ensure_table(n);
    if (table_data == NULL) return 0;   /* failed to allocate */

    seq_t ans = (seq_t)0;
    for (idx_t k = 1; k <= MAX_K; ++k)
    {
        for (idx_t l = 1; l <= MAX_K; ++l)
        {
            if (k + l > 17) continue;
            ans += table_data[idx_of(n, k)] * table_data[idx_of(n, l)];
        }
    }
    return ans;
}
