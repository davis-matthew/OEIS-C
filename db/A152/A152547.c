/*
 * A152547  Triangle, read by rows, derived from Pascal's triangle (see g.f.
 *          and example for generating methods), 0 <= k < A001405(n).
 * https://oeis.org/A152547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   For linear index n, let r be the unique row with sum_{i<r}
 *            C(i,⌊i/2⌋) ≤ n < sum_{i≤r} C(i,⌊i/2⌋). Let k = n - sum_{i<r}
 *            C(i,⌊i/2⌋). Then a(n) = #{j : 0 ≤ j ≤ r, C(r,j) > k}
 * Range:     u8 n <= 7059, u16 n <= 7059, u32 n <= 7059, u64 n <= 7059,
 *            u128 n <= 7059, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static data for rows that are computed on demand. */
static idx_t   rows_allocated = 0;          /* number of rows currently stored */
static idx_t  *row_start     = NULL;        /* cumulative start index of each row */
static idx_t  *row_len       = NULL;        /* number of terms in each row  */
static idx_t **row_binoms    = NULL;        /* binomial coefficients for each row */

/* Add a new row r = rows_allocated. */
static void add_row(void)
{
    idx_t r = rows_allocated;
    /* Grow the row metadata arrays. */
    row_start  = (idx_t *)realloc(row_start,  (r + 1) * sizeof(idx_t));
    row_len    = (idx_t *)realloc(row_len,    (r + 1) * sizeof(idx_t));
    row_binoms = (idx_t **)realloc(row_binoms, (r + 1) * sizeof(idx_t*));

    /* Allocate binomial coefficients for row r. */
    row_binoms[r] = (idx_t *)malloc((r + 1) * sizeof(idx_t));
    if (!row_binoms[r]) abort();

    /* Compute row r's binomial coefficients. */
    row_binoms[r][0] = 1;
    row_binoms[r][r] = 1;
    for (idx_t j = 1; j < r; ++j)
        row_binoms[r][j] = row_binoms[r-1][j-1] + row_binoms[r-1][j];

    /* Row length is binomial(r,⌊r/2⌋). */
    row_len[r] = row_binoms[r][r / 2];

    /* Compute the cumulative start index. */
    if (r == 0)
        row_start[r] = 0;
    else
        row_start[r] = row_start[r-1] + row_len[r-1];

    ++rows_allocated;
}

/* Ensure rows are available up to reach linear index n. */
static void ensure_rows_for_index(idx_t n)
{
    while (rows_allocated == 0 ||
           n >= row_start[rows_allocated-1] + row_len[rows_allocated-1])
        add_row();
}

seq_t a(idx_t n)
{
    /* Make sure we have computed enough rows. */
    ensure_rows_for_index(n);

    /* Find the row containing index n. */
    idx_t r = 0;
    while (n >= row_start[r] + row_len[r])
        ++r;

    /* Offset within that row. */
    idx_t k = n - row_start[r];

    /* Count how many binomial coefficients in row r exceed k. */
    idx_t cnt = 0;
    for (idx_t j = 0; j <= r; ++j)
        if (row_binoms[r][j] > k)
            ++cnt;

    return (seq_t)cnt;
}
