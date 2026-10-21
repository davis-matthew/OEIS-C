/*
 * A079641  Matrix product of Stirling2-triangle A008277(n,k) and unsigned
 *          Stirling1-triangle |A008275(n,k)|.
 * https://oeis.org/A079641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = row‑major flatten of the triangle T(n,k) = Sum_{i=k..n}
 *            Stirling2(n,i) * |Stirling1(i,k)| (OEIS A079641)
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Internal storage for Stirling numbers and the flattened sequence */
static seq_t **s1_rows = NULL;          /* S1 rows, indexed by n (1‑based) */
static size_t rows_cap = 0;             /* allocated capacity for s1_rows */
static size_t max_rows = 0;             /* highest row computed */

static seq_t *s2_prev = NULL;           /* S2 values for previous row */
static size_t s2_prev_len = 0;          /* length of s2_prev (current row+1) */

static seq_t *flat = NULL;              /* flattened triangle values */
static size_t flat_len = 0;             /* number of values stored */
static size_t flat_cap = 0;             /* allocated capacity of flat */

/* Utility: ensure s1_rows can hold up to index 'cap-1' */
static void grow_rows_cap(size_t cap)
{
    if (rows_cap >= cap) return;
    size_t new_cap = rows_cap ? rows_cap * 2 : 4;
    if (new_cap < cap) new_cap = cap;
    s1_rows = (seq_t **)realloc(s1_rows, new_cap * sizeof(seq_t *));
    for (size_t i = rows_cap; i < new_cap; ++i) s1_rows[i] = NULL;
    rows_cap = new_cap;
}

/* Utility: ensure the flattened array has room for 'needed' elements */
static void grow_flat(size_t needed)
{
    if (flat_cap >= needed) return;
    size_t new_cap = flat_cap ? flat_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;
    flat = (seq_t *)realloc(flat, new_cap * sizeof(seq_t));
    flat_cap = new_cap;
}

/* Compute all rows up to and including 'target_row' */
static void ensure_rows(idx_t target_row)
{
    while (max_rows < target_row) {
        idx_t n = max_rows + 1;           /* next row to compute          */
        /* Allocate storage for S1 row n */
        grow_rows_cap(n + 1);             /* need index n                 */
        seq_t *s1_row = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));

        /* Compute S1(n,k) for k=1..n */
        if (n == 1) {
            s1_row[1] = 1;                /* base case S1(1,1)=1         */
        } else {
            const seq_t *prev_s1 = s1_rows[max_rows];  /* S1(n-1,k)   */
            s1_row[1] = (seq_t)(n - 1) * prev_s1[1];
            for (idx_t k = 2; k <= n; ++k) {
                seq_t part1 = prev_s1[k - 1];
                seq_t part2 = (k <= n - 1) ? (seq_t)(n - 1) * prev_s1[k] : 0;
                s1_row[k] = part1 + part2;
            }
        }
        s1_rows[n] = s1_row;

        /* Allocate and compute S2(n,k) for k=1..n */
        seq_t *s2_curr = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
        if (n == 1) {
            s2_curr[1] = 1;                /* base case S2(1,1)=1         */
        } else {
            for (idx_t k = 1; k <= n; ++k) {
                seq_t part1 = (k <= n - 1) ? (seq_t)k * s2_prev[k] : 0;
                seq_t part2 = (k > 1) ? s2_prev[k - 1] : 0;
                s2_curr[k] = part1 + part2;
            }
        }

        /* Compute and store T(n,k) for k=1..n */
        grow_flat(flat_len + (size_t)n);
        for (idx_t k = 1; k <= n; ++k) {
            seq_t sum = 0;
            for (idx_t i = k; i <= n; ++i) {
                sum += s2_curr[i] * s1_rows[i][k];
            }
            flat[flat_len++] = sum;
        }

        /* Prepare for next iteration */
        free(s2_prev);
        s2_prev = s2_curr;
        s2_prev_len = n + 1;
        max_rows = n;
    }
}

/* Public sequence accessor */
seq_t a(idx_t n)
{
    if (n < 1u) return (seq_t)0;      /* undefined for n<1 */

    /* Ensure enough terms are computed in the flat array */
    if (flat_len < n) {
        /* Determine how many rows are needed to reach n terms.
           triangular number r*(r+1)/2 >= n   */
        idx_t r = 1;
        while ((r * (r + 1) / 2) < n) ++r;
        ensure_rows(r);
    }
    return flat[n - 1];
}
