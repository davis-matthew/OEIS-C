/*
 * A209558  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A209557; see the Formula section.
 * https://oeis.org/A209558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A209558 – triangle of coefficients of v(n,x) * * Recurrence: *
 *            u(1,x) = 1, v(1,x) = 1 * u(n,x) = x*u(n‑1,x) + v(n‑1,x) * v(n,x)
 *            = u(n‑1,x) + 2*x*v(n‑1,x) + 1 * * a(n) is the n‑th coefficient
 *            when the rows of the v‑polynomials are * listed row‑by‑row
 *            (constant term first). Offset: 1 (a(1)=1)
 * Range:     u8 n <= 32, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the n‑th term of A209558.  n is 1‑based. */
seq_t a(idx_t n)
{
    /* No result for indices below the offset. */
    if (n == 0)
        return 0;

    /* Static storage for rows that have been generated once. */
    static size_t max_row_computed = 0;        /* highest row that has been built */
    static seq_t **v_rows = NULL;             /* v_rows[row] points to array of coeffs of v(row,x) */
    static size_t *row_sizes = NULL;          /* row_sizes[row] = number of coefficients in row */
    static seq_t *prev_u = NULL;              /* u array for the latest computed row */
    static seq_t *prev_v = NULL;              /* v array for the latest computed row */
    static size_t prev_len = 0;               /* length of the latest computed row */

    /* First call – initialise for row 1 */
    if (max_row_computed == 0) {
        /* Allocate space for one row (index 1).  Extra slot for 1‑based indexing. */
        v_rows = (seq_t **)malloc((size_t)2 * sizeof(seq_t *));
        row_sizes = (size_t *)malloc((size_t)2 * sizeof(size_t));
        if (!v_rows || !row_sizes)
            return 0;                 /* allocation failure – return 0 */
        /* row 1 has a single coefficient (0 degree) which is 1. */
        v_rows[1] = (seq_t *)malloc(sizeof(seq_t));
        if (!v_rows[1]) return 0;
        v_rows[1][0] = (seq_t)1;
        row_sizes[1] = 1;

        /* Initialise previous u and v arrays for generating subsequent rows. */
        prev_u = (seq_t *)malloc(sizeof(seq_t));
        prev_v = (seq_t *)malloc(sizeof(seq_t));
        if (!prev_u || !prev_v)
            return 0;
        prev_u[0] = (seq_t)1;
        prev_v[0] = (seq_t)1;
        prev_len = 1;
        max_row_computed = 1;
    }

    /* Determine which row contains the n‑th coefficient. */
    /* The cumulative number of terms up to row r is r*(r+1)/2. */
    size_t target_row = 1;
    while ((size_t)target_row * (target_row + 1u) / 2u < (size_t)n)
        ++target_row;

    /* Generate rows up to target_row if not already done. */
    for (size_t row = max_row_computed + 1; row <= target_row; ++row) {
        size_t len_cur = row;
        seq_t *cur_u = (seq_t *)malloc(len_cur * sizeof(seq_t));
        seq_t *cur_v = (seq_t *)malloc(len_cur * sizeof(seq_t));
        if (!cur_u || !cur_v)
            return 0;                 /* allocation failure – return 0 */

        /* Build u(row,x) = x*u(row‑1,x) + v(row‑1,x) */
        cur_u[0] = prev_v[0];
        for (size_t k = 1; k < len_cur; ++k) {
            seq_t part1 = prev_u[k - 1];
            seq_t part2 = (k < prev_len) ? prev_v[k] : (seq_t)0;
            cur_u[k] = part1 + part2;
        }

        /* Build v(row,x) = u(row‑1,x) + 2*x*v(row‑1,x) + 1 */
        cur_v[0] = prev_u[0] + (seq_t)1;
        for (size_t k = 1; k < len_cur; ++k) {
            seq_t part_u = (k < prev_len) ? prev_u[k] : (seq_t)0;
            seq_t part_v = prev_v[k - 1];
            cur_v[k] = part_u + (part_v + part_v);   /* part_v * 2 */
        }

        /* Store cur_v in the static array of rows. */
        size_t new_size = row + 1;                 /* 1‑based indexing */
        v_rows  = (seq_t **)realloc(v_rows,  new_size * sizeof(seq_t *));
        row_sizes = (size_t *)realloc(row_sizes, new_size * sizeof(size_t));
        if (!v_rows || !row_sizes) {
            free(cur_u);
            free(cur_v);
            return 0;
        }

        v_rows[row]  = cur_v;
        row_sizes[row] = len_cur;

        /* Prepare for next iteration: cur becomes prev. */
        free(prev_u);
        free(prev_v);
        prev_u = cur_u;
        prev_v = cur_v;
        prev_len = len_cur;
    }

    max_row_computed = target_row;

    /* Compute zero‑based column index within the target row. */
    idx_t before = (idx_t)(target_row - 1u) * (idx_t)target_row / 2u;
    idx_t offset = n - before - 1u;           /* 0‑based position in the row */

    return v_rows[target_row][offset];
}
