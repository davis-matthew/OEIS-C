/*
 * A226392  Triangle with first column identical to 1 and the other entries
 *          defined by the sum of entries above and to the left.
 * https://oeis.org/A226392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 42, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * A226392 – Triangle with first column = 1 and other entries defined by the sum of entries
 *          above and to the left.  The sequence a(n) is the flattened row‑major
 *          traversal of the triangle.
 * Formula:  a(n) = T(r,c) where (r,c) is obtained from n by row‑major indexing.
 *          T(r,0)=1,
 *          T(r,k)= sum_{c<k} T(r,c) + sum_{k<=s<r} T(s,k)   (k>0).
 */

#include <stdlib.h>   /* malloc, realloc, free, abort */
#include <stddef.h>   /* size_t */

/* Static storage for the triangle rows and column sums */
static seq_t **T_rows     = NULL;   /* array of pointers to each row */
static size_t  T_rows_cap = 0;      /* allocated capacity for T_rows    */
static idx_t   T_rows_used = 0;     /* number of rows already computed  */

static seq_t *col_sum     = NULL;   /* column sums of previous rows    */
static size_t col_sum_cap = 0;      /* allocated capacity for col_sum   */

/* Ensure the rows array can hold indices up to up_to (inclusive) */
static void grow_rows_to(idx_t up_to)
{
    if (T_rows_cap <= up_to) {
        size_t new_cap = T_rows_cap ? T_rows_cap * 2 : 8;
        while (new_cap <= up_to) new_cap *= 2;
        seq_t **tmp = (seq_t**)realloc(T_rows, new_cap * sizeof(seq_t*));
        if (!tmp) abort();                     /* out of memory */
        for (size_t i = T_rows_cap; i < new_cap; ++i) tmp[i] = NULL;
        T_rows = tmp;
        T_rows_cap = new_cap;
    }
}

/* Ensure the col_sum array can hold indices up to up_to (inclusive) */
static void grow_cols_to(idx_t up_to)
{
    if (col_sum_cap <= up_to) {
        size_t new_cap = col_sum_cap ? col_sum_cap * 2 : 8;
        while (new_cap <= up_to) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(col_sum, new_cap * sizeof(seq_t));
        if (!tmp) abort();                      /* out of memory */
        for (size_t i = col_sum_cap; i < new_cap; ++i) tmp[i] = 0;
        col_sum = tmp;
        col_sum_cap = new_cap;
    }
}

/* Compute the row with index i (0‑based) and store it. */
static void compute_row(idx_t i)
{
    /* Ensure there is space for this row pointer */
    grow_rows_to(i);

    /* Allocate current row */
    seq_t *row = (seq_t*)malloc((i + 1) * sizeof(seq_t));
    if (!row) abort();                     /* out of memory */

    /* Ensure col_sum can hold index i */
    grow_cols_to(i);

    seq_t left_sum = 0;  /* running sum of current row */

    for (idx_t k = 0; k <= i; ++k) {
        seq_t val;
        if (k == 0) {
            val = 1;               /* base case */
        } else {
            val = left_sum + col_sum[k];
        }
        row[k] = val;
        left_sum += val;
        col_sum[k] += val;          /* update column sum for future rows */
    }

    T_rows[i] = row;
    ++T_rows_used;                  /* one more row computed */
}

/* Ensure that all rows up to `up_to` have been computed. */
static void ensure_rows(idx_t up_to)
{
    while (T_rows_used <= up_to) {
        compute_row(T_rows_used);
    }
}

/* Helper to find row r and column c for flattened index n. */
static void index_to_rc(idx_t n, idx_t *r_out, idx_t *c_out)
{
    /* Find smallest r with (r+1)(r+2)/2 > n */
    idx_t r = 0;
    for (;;) {
        unsigned __int128 tri = (unsigned __int128)(r + 1) * (r + 2) / 2;
        if (tri > n) break;
        ++r;
    }
    *r_out = r;
    *c_out = n - r * (r + 1) / 2;   /* offset within the row */
}

/* Public function */
seq_t a(idx_t n)
{
    idx_t r, c;
    index_to_rc(n, &r, &c);

    /* Compute rows up to r if necessary */
    ensure_rows(r);

    return T_rows[r][c];
}
