/*
 * A026615  Triangle read by rows: T(n,0) = T(n,n) = 1 for n >= 0, T(n,1) =
 *          T(n,n-1) = 2*n-1 for n >= 1, T(n,k) = T(n-1,k-1) + T(n-1,k) for 2
 *          <= k <= n-2 and n >= 4.
 * https://oeis.org/A026615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where i*(i+1)/2 ≤ n < (i+1)*(i+2)/2. Triangle
 *            rules: T(i,0)=T(i,i)=1 T(i,1)=T(i,i-1)=2*i-1 for i ≥ 1
 *            T(i,k)=T(i-1,k-1)+T(i-1,k) for 2 ≤ k ≤ i-2 and i ≥ 4
 * Range:     u8 n <= 58, u16 n <= 178, u32 n <= 644, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t **triangle_rows = NULL;   /* row i array */
static idx_t  max_computed_row = 0;    /* largest row fully computed */
static bool   initialized = false;    /* row 0 computed flag */

/* Build one row r of the triangle.  The previous row must already be present. */
static void compute_row(idx_t r)
{
    size_t row_len = (size_t)(r + 1);
    seq_t *row = (seq_t *)malloc(row_len * sizeof(seq_t));
    if (!row) abort();

    row[0] = (seq_t)1;
    row[r] = (seq_t)1;

    if (r >= 1) {
        seq_t odd = (seq_t)((seq_t)r * 2 - 1);
        row[1] = odd;
        row[r - 1] = odd;
    }

    if (r >= 4) {
        for (idx_t k = 2; k <= r - 2; ++k) {
            row[k] = triangle_rows[r - 1][k - 1] + triangle_rows[r - 1][k];
        }
    }

    triangle_rows[r] = row;
}

/* Ensure that rows 0 … needed are computed. */
static void ensure_rows(idx_t needed)
{
    if (!triangle_rows) {
        /* First call – allocate space for rows 0 … needed. */
        triangle_rows = (seq_t **)malloc((size_t)(needed + 1) * sizeof(seq_t *));
        if (!triangle_rows) abort();

        /* Compute row 0. */
        compute_row(0);
        max_computed_row = 0;
    } else if (needed > max_computed_row) {
        /* Grow the row array if necessary. */
        seq_t **tmp = (seq_t **)realloc(triangle_rows,
                                        (size_t)(needed + 1) * sizeof(seq_t *));
        if (!tmp) abort();
        triangle_rows = tmp;

        /* Compute the missing rows. */
        for (idx_t r = max_computed_row + 1; r <= needed; ++r)
            compute_row(r);
        max_computed_row = needed;
    }

    initialized = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Find the row index i such that i*(i+1)/2 ≤ n < (i+1)*(i+2)/2. */
    idx_t i = 0;
    while (true) {
        unsigned __int128 next = (unsigned __int128)(i + 1) * (i + 2) / 2;
        if (next > n) break;
        i++;
    }

    /* Column index j within that row. */
    unsigned __int128 tri128 = (unsigned __int128)i * (i + 1) / 2;
    idx_t j = n - (idx_t)tri128;

    /* Make sure rows are available. */
    if (!initialized || i > max_computed_row)
        ensure_rows(i);

    return triangle_rows[i][j];
}
