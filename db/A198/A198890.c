/*
 * A198890  Irregular triangle read by rows: row n gives expansion of g.f. for
 *          descending plane partitions of order n with no special parts and
 *          weight equal to sum of the parts.
 * https://oeis.org/A198890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 146, u16 n <= 146, u32 n <= 146, u64 n <= 146,
 *            u128 n <= 146, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A198890
 *  Irregular triangle read by rows.
 *
 *  The triangle T(n,k) (n ≥ 0, k ≥ 0) is defined by:
 *      T(0,0) = 1;   T(1,0) = 1.
 *      T(n,k) = 0                               for k < 0 or
 *                                              k > (1/3)*(n+1)*n*(n-1).
 *      T(n,k) = T(n, k-n) + T(n-1, k) - T(n-1, k-n²)   otherwise
 *  (terms outside the triangle are treated as 0).
 *
 *  a(n) is the n‑th element (1‑indexed) of this triangle read row‑wise.
 *  The implementation builds rows on demand, stores each row, and
 *  retrieves the required entry in amortised O(1) time.
 */

#include <stdlib.h>

/* The types seq_t and idx_t are supplied externally; no
 * definition is required here.  All arithmetic uses only the
 * operators permitted for seq_t by the problem statement. */
static seq_t **rows = NULL;          /* pointer to each stored row (rows[0] is T(0,0)) */
static idx_t  *row_sizes = NULL;     /* number of elements in each row (max_k+1)    */
static idx_t  *row_start = NULL;     /* cumulative number of elements up to the
                                       start of this row (including all
                                       previous rows).                       */
static idx_t   row_count = 0;        /* number of rows currently stored         */
static size_t  alloc_cap = 0;        /* capacity of the three pointer arrays    */

/* Ensure that the arrays that keep row information are large enough. */
static void
ensure_cap(size_t needed)
{
    if (needed <= alloc_cap)
        return;
    size_t new_cap = alloc_cap ? alloc_cap * 2 : 1;
    while (new_cap < needed)
        new_cap *= 2;

    rows       = (seq_t **)realloc(rows,       new_cap * sizeof(seq_t *));
    row_sizes  = (idx_t *)realloc(row_sizes,  new_cap * sizeof(idx_t));
    row_start  = (idx_t *)realloc(row_start,  new_cap * sizeof(idx_t));
    alloc_cap = new_cap;
}

/* Compute row m (m = row_count) and append it to the storage. */
static void
compute_next_row(void)
{
    idx_t m = row_count;                       /* next row index to compute       */
    /* length of the new row: max_k + 1 = (m+1)*m*(m-1)/3 + 1. */
    idx_t max_k = (m + 1) * m * (m - 1) / 3;
    size_t len = (size_t)(max_k + 1);

    ensure_cap(row_count + 1);

    seq_t *row = (seq_t *)malloc(len * sizeof(seq_t));
    const seq_t *prev = rows[m - 1];
    idx_t prev_size = row_sizes[m - 1];

    /* Row 0 is trivial; for all other rows use the recurrence. */
    if (m == 0) {
        row[0] = (seq_t)1;  /* T(0,0) = 1 */
    } else {
        for (size_t k = 0; k < len; ++k) {
            seq_t val = (seq_t)0;

            if (k >= (size_t)m)            /* T(m, k-n) */
                val += row[k - m];

            if (k < (size_t)prev_size)     /* T(m-1, k) */
                val += prev[k];

            idx_t n_sq = m * m;
            if (k >= n_sq) {
                idx_t idx = k - n_sq;
                if (idx < (size_t)prev_size) {
                    seq_t sub = prev[idx];
                    if (sub > val)          /* safety: avoid unsigned underflow */
                        val = (seq_t)0;
                    else
                        val -= sub;
                }
            }

            row[k] = val; /* values are 0 or 1, but the code works for any seq_t */
        }
    }

    /* Record the new row and its metadata. */
    rows[row_count]      = row;
    row_sizes[row_count] = (idx_t)len;
    if (row_count == 0)
        row_start[row_count] = 0;               /* row 0 starts at index 0 (not in seq) */
    else
        row_start[row_count] = row_start[row_count - 1] + row_sizes[row_count - 1];
    ++row_count;
}

/* Ensure that rows 0 … needed are produced so that the target linear
 * index is within an existing row. */
static void
prepare_rows_until(idx_t target_index)
{
    /* target_index is zero‑based with row0 occupying index 0,
     * but the sequence starts at index 1. */
    while (target_index >= row_start[row_count - 1] + row_sizes[row_count - 1]) {
        compute_next_row();
    }
}

/* Public function required by the harness. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                /* undefined; return 0 for safety */

    /* Initialise with row 0 (handled lazily). */
    if (row_count == 0)
        compute_next_row();             /* after this, row 0 exists */

    /* Linear target index in the combined row sequence (row0 included). */
    idx_t target = n;

    /* Build rows until the target lies inside an existing row. */
    prepare_rows_until(target);

    /* Locate the row that contains the target. */
    idx_t r = 0;
    while (target >= row_start[r] + row_sizes[r])
        ++r;

    idx_t col = target - row_start[r];
    return rows[r][col];
}
