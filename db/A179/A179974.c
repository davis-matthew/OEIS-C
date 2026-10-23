/*
 * A179974  Irregular triangle T(n,k) = (n-A049085(n,k))!, read by rows, 1 <=
 *          k <= A000041(n).
 * https://oeis.org/A179974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = factorial(row_of_n - number_of_parts_of_partition) where
 *            partitions are listed in the reversed Abramowitz–Stegun order
 *            (A036036 reversed) and the rows are read one after another
 * Range:     u8 n <= 43, u16 n <= 137, u32 n <= 506, u64 n <= 2713,
 *            u128 n <= 2713, big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#include "../utils/factorial.h"

/* Cached data for partitions of each row. */
static int **row_parts = NULL;        /* row_parts[r] -> array of part counts of row r, reversed order */
static size_t *row_lens = NULL;       /* row_lens[r] -> number of partitions of r */
static size_t row_cnt  = 0;           /* number of rows already generated */
static size_t row_cap  = 0;           /* allocation capacity for row_parts and row_lens */

/* Append a partition result to *arr (dynamic array). */
static void rec_part(idx_t rem, idx_t max_part, int parts,
                     int **arr, size_t *len, size_t *cap)
{
    if (rem == 0) {
        if (*len == *cap) {
            *cap = (*cap == 0) ? 16 : *cap << 1;
            *arr = (int *)realloc(*arr, (*cap) * sizeof(int));
        }
        (*arr)[*len] = parts;
        ++(*len);
        return;
    }
    for (idx_t part = (rem < max_part ? rem : max_part); part > 0; --part) {
        rec_part(rem - part, part, parts + 1, arr, len, cap);
    }
}

/* Generate partition data for row r (0‑based). */
static void generate_row(idx_t r)
{
    if (r == 0) {                     /* the empty partition */
        int *arr = (int *)malloc(sizeof(int));
        arr[0] = 0;
        if (row_cap <= r) {
            row_cap = (row_cap == 0) ? r + 1 : row_cap;
            while (row_cap <= r) row_cap <<= 1;
            row_parts = (int **)realloc(row_parts, row_cap * sizeof(int *));
            row_lens  = (size_t *)realloc(row_lens,  row_cap * sizeof(size_t));
        }
        row_parts[r] = arr;
        row_lens[r]  = 1;
        if (row_cnt <= r) row_cnt = r + 1;
        return;
    }

    int *arr = NULL;
    size_t len = 0, cap = 0;
    rec_part(r, r, 0, &arr, &len, &cap);

    if (row_cap <= r) {
        row_cap = (row_cap == 0) ? r + 1 : row_cap;
        while (row_cap <= r) row_cap <<= 1;
        row_parts = (int **)realloc(row_parts, row_cap * sizeof(int *));
        row_lens  = (size_t *)realloc(row_lens,  row_cap * sizeof(size_t));
    }
    row_parts[r] = arr;
    row_lens[r]  = len;
    if (row_cnt <= r) row_cnt = r + 1;
}

/* Ensure rows 0…r are generated. */
static void ensure_rows_up_to(idx_t r)
{
    for (idx_t i = row_cnt; i <= r; ++i)
        generate_row(i);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Find the row r that contains the n‑th term. */
    idx_t cum = 0;
    idx_t r = 0;
    while (1) {
        ensure_rows_up_to(r);
        size_t p = row_lens[r];
        if (cum + p > n) break;
        cum += p;
        ++r;
    }

    /* Position within the row (0‑based). */
    idx_t pos = n - cum;

    /* Reversed order: last partition first. */
    idx_t rev_idx = row_lens[r] - 1 - pos;

    /* Number of parts of that partition. */
    int parts = row_parts[r][rev_idx];

    /* Compute factorial of (r - parts). */
    return factorial((idx_t)(r - parts));
}
