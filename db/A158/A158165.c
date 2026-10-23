/*
 * A158165  Number of nondecreasing integer sequences of length 31 with sum
 *          zero and sum of absolute values 2n.
 * https://oeis.org/A158165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of non‑decreasing integer sequences of length 31
 *            with total sum 0 and total absolute sum 2n. For n ≥ 1, a(n) =
 *            Σ_{r=2}^{min(31,2n)} Σ_{l=max(1,r−n)}^{min(n,r−1)}
 *            p(n,l)·p(n,r−l) where p(n,r) is the number of partitions of n
 *            into exactly r parts
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 43, u64 n <= 149, u128 n <= 235,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---- standard headers ---------------------------------------------------- */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* ---- internal constants -------------------------------------------------- */
#define MAX_R 32            /* number of columns (0 … 31) */

/* ---- static storage for partition table --------------------------------- */
static seq_t *partition_tbl = NULL;   /* rows × MAX_R entries */
static idx_t partition_max_n = 0;     /* largest i that has been computed */

/* ---- ensure that partition_tbl contains rows 0 … n ------------------------ */
static void ensure_partitions(idx_t n)
{
    /* allocate initial empty table */
    if (partition_tbl == NULL) {
        partition_tbl = (seq_t *)calloc(1 * MAX_R, sizeof(seq_t));
        if (!partition_tbl) abort();
        partition_tbl[0] = 1;               /* p(0,0) = 1 */
        partition_max_n = 0;
    }
    if ((idx_t)partition_max_n >= n) return;   /* already sufficient */

    /* enlarge table to hold up to n rows (rows are 0 … n) */
    size_t old_rows = (size_t)(partition_max_n + 1);
    size_t new_rows = (size_t)(n + 1);
    seq_t *new_tbl = (seq_t *)realloc(partition_tbl, new_rows * MAX_R * sizeof(seq_t));
    if (!new_tbl) abort();
    partition_tbl = new_tbl;

    /* zero newly added rows */
    for (size_t i = old_rows; i < new_rows; ++i)
        for (size_t r = 0; r < MAX_R; ++r)
            partition_tbl[i * MAX_R + r] = 0;

    /* compute partitions for each new row i = old_rows … n */
    for (size_t i = old_rows; i <= n; ++i) {
        partition_tbl[i * MAX_R + 0] = 0;      /* p(i,0) = 0 for i > 0 */
        for (size_t r = 1; r < MAX_R; ++r) {
            seq_t a = partition_tbl[(i - 1) * MAX_R + (r - 1)];
            seq_t b = (i >= r) ? partition_tbl[(i - r) * MAX_R + r] : 0;
            partition_tbl[i * MAX_R + r] = a + b;
        }
    }
    partition_max_n = n;
}

/* ---- main sequence function --------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* not part of the defined range */

    ensure_partitions(n);

    seq_t result = 0;
    /* maximum r we need to look at */
    size_t r_max = 31;
    if ((size_t)(2 * n) < r_max) r_max = (size_t)(2 * n);

    for (size_t r = 2; r <= r_max; ++r) {
        /* lower bound for l: l ≥ 1 and r−l ≤ n  →  l ≥ r−n */
        size_t low = (r > (size_t)n) ? (r - (size_t)n) : 1;
        /* upper bound for l: l ≤ n and l ≤ r−1 */
        size_t high = (( (size_t)n < r - 1) ? (size_t)n : r - 1);
        if (low > high) continue;

        for (size_t l = low; l <= high; ++l) {
            size_t k = r - l;
            seq_t prod = partition_tbl[(size_t)n * MAX_R + l] *
                         partition_tbl[(size_t)n * MAX_R + k];
            result += prod;
        }
    }

    return result;
}
