/*
 * A195822  Triangle read by rows in which row n lists the Dyson's ranks of
 *          all partitions of n that do not contain 1 as a part, in
 *          nonincreasing order.
 * https://oeis.org/A195822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabf
 * Formula:   not stated
 * Range:     i8 n <= 77, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   rec_partitions
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A195822 (Dyson ranks of partitions with no part 1) by
   generating all such partitions for a given row n, sorting their
   ranks in non‑increasing order, and returning the nth element of the
   flattened triangle. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/rec_partitions.h"

/* Comparator for descending order of seq_t values. */
static int cmp_desc(const void *pa, const void *pb)
{
    const seq_t *a = (const seq_t *)pa;
    const seq_t *b = (const seq_t *)pb;
    if (*a < *b) return 1;
    if (*a > *b) return -1;
    return 0;
}

/* Structures to cache already generated rows.  1‑based indexing. */
static seq_t **row_items = NULL;   /* ranks for each row */
static size_t  *row_len   = NULL;   /* number of ranks */
static idx_t   *row_cum   = NULL;   /* cumulative counts after this row */
static idx_t    rows_capacity = 0;   /* allocated space for rows */
static idx_t    rows_count    = 0;   /* rows that have been generated */

/* Grow the row cache to hold at least 'cap' rows (1‑based). */
static inline void ensure_rows_capacity(idx_t cap)
{
    if (cap <= rows_capacity)
        return;
    idx_t new_cap = rows_capacity ? rows_capacity * 2 : cap;
    while (new_cap < cap)
        new_cap <<= 1;
    row_items = (seq_t **)realloc(row_items, (new_cap + 1) * sizeof(seq_t *));
    row_len   = (size_t  *)realloc(row_len,   (new_cap + 1) * sizeof(size_t));
    row_cum   = (idx_t   *)realloc(row_cum,   (new_cap + 1) * sizeof(idx_t));
    rows_capacity = new_cap;
}

/* Generate all ranks for partitions of n (n >= 2), sort them desc. */
static void generate_ranks(idx_t n, seq_t **out_arr, size_t *out_len)
{
    seq_t *arr = NULL;
    size_t len = 0, cap = 0;
    rec_partitions(n, n, 0, 0, &arr, &len, &cap);
    qsort(arr, len, sizeof(seq_t), cmp_desc);
    *out_arr = arr;
    *out_len = len;
}

/* Store a newly computed row. */
static void append_row(idx_t r, seq_t *arr, size_t len)
{
    ensure_rows_capacity(r);
    row_items[r] = arr;
    row_len[r]   = len;
    if (r == 1)
        row_cum[r] = (idx_t)len;
    else
        row_cum[r] = row_cum[r - 1] + (idx_t)len;
    if (r > rows_count)
        rows_count = r;
}

/* The main sequence function.  a(n) returns the nth term of the
   flattened triangle. */
seq_t a(idx_t n)
{
    /* Generate rows until the cumulative count covers the desired index. */
    while (rows_count == 0 || n > row_cum[rows_count]) {
        idx_t r = rows_count + 1;
        if (r == 1) {                                     /* special first row */
            seq_t *arr = (seq_t *)malloc(sizeof(seq_t));
            if (!arr) return (seq_t)0;                     /* out of memory */
            arr[0] = (seq_t)0;
            append_row(r, arr, 1);
        } else {
            seq_t *arr;
            size_t len;
            generate_ranks(r, &arr, &len);
            append_row(r, arr, len);
        }
    }

    /* Locate the row that contains the nth term. */
    idx_t r = 1;
    while (n > row_cum[r])
        ++r;

    /* Position within that row (0‑based). */
    idx_t prev_cum = (r == 1) ? 0 : row_cum[r - 1];
    idx_t pos = n - prev_cum - 1;

    return row_items[r][pos];
}
