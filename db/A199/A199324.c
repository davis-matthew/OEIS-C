/*
 * A199324  Triangle T(n,k), read by rows, given by
 *          (-1,1,-1,0,0,0,0,0,0,0,...) DELTA (1,0,0,0,0,0,0,0,0,0,...) where
 *          DELTA is the operator defined in A084938.
 * https://oeis.org/A199324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 69, i16 n <= 69, i32 n <= 69, i64 n <= 69, i128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:      T(n,k) = T(n‑1,k‑1) + T(n‑2,k‑1) – T(n‑1,k) – T(n‑2,k)
 * Initial condition: T(0,0) = 1.
 * The sequence a(n) is the row‑major serialization of the triangle T.
 * Example rows:
 *   1
 *   -1, 1
 *   0, -1, 1
 */

#include <stdlib.h>
#include <stdint.h>
#ifdef __cplusplus
#  include <new>
#endif

/* Static storage for rows and linear (row‑major) list of values. */
static seq_t **rows = NULL;          /* rows[r] → array of r+1 entries      */
static idx_t  rows_allocated = 0;    /* number of rows already computed     */

static seq_t *seq_values = NULL;     /* linear list of a(n) values          */
static idx_t  seq_size_current = 0;  /* number of values stored so far      */
static idx_t  seq_cap = 0;           /* current capacity of seq_values      */

/* Ensure seq_values can hold index idx (zero‑based). */
static void ensure_seq_capacity(idx_t idx)
{
    if (idx < seq_cap) return;           /* already enough space */

    size_t new_cap = seq_cap ? seq_cap : 1;
    while (new_cap <= (size_t)idx) new_cap <<= 1;

#if defined(__cplusplus)
    seq_t *new_seq = new seq_t[new_cap];
#else
    seq_t *new_seq = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif
    for (idx_t i = 0; i < seq_size_current; ++i) {
        new_seq[i] = seq_values[i];
    }
#if defined(__cplusplus)
    delete[] seq_values;
#else
    free(seq_values);
#endif
    seq_values = new_seq;
    seq_cap    = (idx_t)new_cap;
}

/* Allocate a row of length len. */
static seq_t *alloc_row(size_t len)
{
#if defined(__cplusplus)
    return new seq_t[len];
#else
    return (seq_t *)malloc(len * sizeof(seq_t));
#endif
}

/*
 * Ensure that rows[0..r] exist and that seq_values contains all
 * linear indices up to the end of row r (inclusive).
 */
static void ensure_rows_up_to(size_t r)
{
    if (rows_allocated > r) return;  /* already computed enough */

    size_t new_size = rows_allocated ? rows_allocated : 1;
    while (new_size <= r) new_size <<= 1;

    rows = (seq_t **)realloc(rows, new_size * sizeof(seq_t *));
    for (size_t i = rows_allocated; i <= r; ++i) {
        rows[i] = alloc_row(i + 1);   /* allocate row i (length i+1) */
        for (idx_t k = 0; k <= (idx_t)i; ++k) {
            seq_t val = 0;
            /* Base case */
            if (i == 0 && k == 0) {
                val = 1;
            } else {
                /* T(i-1,k-1) */
                if (i >= 1 && k >= 1) {
                    val += rows[i - 1][k - 1];
                }
                /* T(i-2,k-1) – only if k-1 is a valid index in row i-2 */
                if (i >= 2 && k >= 1 && k <= i - 1) {
                    val += rows[i - 2][k - 1];
                }
                /* T(i-1,k) */
                if (i >= 1 && k <= i - 1) {
                    val -= rows[i - 1][k];
                }
                /* T(i-2,k) */
                if (i >= 2 && k <= i - 2) {
                    val -= rows[i - 2][k];
                }
            }
            rows[i][k] = val;

            /* Append to linear list */
            ensure_seq_capacity(seq_size_current);
            seq_values[seq_size_current++] = val;
        }
    }
    rows_allocated = r + 1;
}

/*
 * Convert a linear index n to its corresponding row r and column k
 * within that row.  The triangle is enumerated row‑by‑row, each row
 * containing (r+1) entries.
 */
static void linear_index_to_rc(idx_t n, size_t *r_out, idx_t *k_out)
{
    size_t r = 0;
    /* Find the smallest r such that n < (r+1)(r+2)/2. */
    while (((r + 1) * (r + 2)) / 2 <= (size_t)n) ++r;
    size_t start = (r * (r + 1)) / 2;
    *r_out = r;
    *k_out = (idx_t)(n - start);
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n < seq_size_current) return seq_values[n];

    size_t r;
    idx_t k;
    linear_index_to_rc(n, &r, &k);
    ensure_rows_up_to(r);
    return seq_values[n];
}
