/*
 * A102627  Number of partitions of n into distinct parts in which the number
 *          of parts divides n.
 * https://oeis.org/A102627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{d|n} p_d(n) where p_d(n) = number of partitions of n
 *            into d distinct parts. p_1(n) = 1 for all n ≥ 1. For d ≥ 2:
 *            p_d(n) = p_d(n-d) + p_{d-1}(n-d) (if n ≥ d; otherwise p_d(n) =
 *            0) The table p_d(n) is built once and reused for all calls up to
 *            the maximum n requested
 * Range:     u8 n <= 35, u16 n <= 89, u32 n <= 259, u64 n <= 839,
 *            u128 n <= 1000, big any n
 * Helpers:   needed_rows
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "../utils/needed_rows.h"

/* static data used to cache the p_d(n) table */
static seq_t *p_table = NULL;   /* flattened array: p_table[d*(cols)+n] */
static idx_t  cols   = 0;       /* current number of columns (max_n+1) */
static idx_t  rows   = 0;       /* current number of rows    (max_d+1) */

static void recompute_table(idx_t n)
{
    idx_t new_cols = n + 1;
    idx_t new_rows = needed_rows(n);

    /* allocate new table */
    seq_t *new_table = (seq_t *)calloc((size_t)new_rows * (size_t)new_cols,
                                         sizeof(seq_t));
    if (!new_table) {
        /* allocation failed – terminate program */
        abort();
    }

    /* initialization for d = 1 : single part partitions */
    for (idx_t x = 1; x < new_cols; ++x) {
        new_table[1 * new_cols + x] = (seq_t)1;
    }

    /* fill DP using the recurrence */
    for (idx_t d = 2; d < new_rows; ++d) {
        for (idx_t x = 1; x < new_cols; ++x) {
            if (x < d) {
                /* minimal sum for d distinct parts is d*(d+1)/2 */
                new_table[d * new_cols + x] = (seq_t)0;
            } else {
                seq_t a = new_table[d * new_cols + (x - d)];
                seq_t b = new_table[(d - 1) * new_cols + (x - d)];
                new_table[d * new_cols + x] = a + b;
            }
        }
    }

    /* replace old table */
    free(p_table);
    p_table = new_table;
    cols = new_cols;
    rows = new_rows;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but harmless */

    /* enlarge table if necessary */
    if (n + 1 > cols) {
        recompute_table(n);
    }

    seq_t sum = (seq_t)0;
    idx_t kmax = needed_rows(n) - 1;   /* maximum d that can occur */

    /* iterate over divisors of n */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t e = n / d;

            if (d <= kmax) {
                sum += p_table[d * cols + n];
            }
            if (e != d && e <= kmax) {
                sum += p_table[e * cols + n];
            }
        }
    }
    return sum;
}
