/*
 * A177811  Sub-triangle of Eulerian numbers A008292, taking every 4th term of
 *          every 4th row.
 * https://oeis.org/A177811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, less
 * Formula:   a(n) = Eulerian(4*r + 1, 4*offset) where r is the unique integer
 *            with r*(r+1)/2 ≤ n < (r+1)*(r+2)/2, and offset = n - r*(r+1)/2
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 10, u64 n <= 22, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* --------------------------------------------------------------------
 *  Pre-allocated storage for Eulerian numbers up to row MAX_R.
 *  MAX_R is chosen to cover all rows needed for n up to about 10000.
 * -------------------------------------------------------------------- */
#define MAX_R     600
#define POOL_SIZE ((MAX_R * (MAX_R + 1)) / 2 + 1)

/* Static storage for Eulerian numbers */
static seq_t pool[POOL_SIZE];          /* flattened Eulerian rows */
static idx_t row_start[MAX_R + 1];     /* start index of each row in pool */
static idx_t max_row_computed = 0;     /* highest row computed so far */
static int    initialized   = 0;       /* flag for first call */

/* --------------------------------------------------------------------
 *  Initialise row offset table and row 0.
 * -------------------------------------------------------------------- */
static void init_offsets(void)
{
    idx_t offset = 0;
    for (idx_t R = 0; R <= MAX_R; ++R)
    {
        row_start[R] = offset;
        idx_t len = (R == 0 ? 1 : R);  /* row length: 1 for R=0, else R */
        offset += len;
    }
    pool[row_start[0]] = 1;            /* Eulerian(0,0) = 1 */
    max_row_computed  = 0;
    initialized       = 1;
}

/* --------------------------------------------------------------------
 *  Compute rows up to R_needed (inclusive) using the Eulerian recurrence.
 * -------------------------------------------------------------------- */
static void ensure_rows(idx_t R_needed)
{
    if (!initialized)
        init_offsets();

    if (R_needed <= max_row_computed)
        return;   /* already computed */

    for (idx_t R = max_row_computed + 1; R <= R_needed; ++R)
    {
        idx_t len   = (R == 0 ? 1 : R);            /* length of this row */
        seq_t *curr = &pool[row_start[R]];        /* target storage   */

        if (R == 0)
        {
            curr[0] = 1;
            continue;
        }

        /* previous row */
        seq_t *prev = &pool[row_start[R - 1]];

        for (idx_t k = 0; k < len; ++k)
        {
            if (k == 0 || k == len - 1)
                curr[k] = 1;
            else
                curr[k] = (seq_t)(k + 1) * prev[k] +
                          (seq_t)(R - k) * prev[k - 1];
        }
    }
    max_row_computed = R_needed;
}

/* --------------------------------------------------------------------
 *  Find the sub-triangle row index for a given flattened index n.
 *  r satisfies r*(r+1)/2 ≤ n < (r+1)*(r+2)/2.
 * -------------------------------------------------------------------- */
static inline idx_t row_of_n(idx_t n)
{
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;
    return r;
}

/* --------------------------------------------------------------------
 *  Public sequence function as required.
 * -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* determine sub-triangle row r and position inside that row */
    idx_t r      = row_of_n(n);
    idx_t offset = n - r * (r + 1) / 2;                 /* offset within row r */

    /* corresponding Eulerian row and column */
    idx_t R = 4 * r + 1;                                 /* Eulerian row index */
    idx_t k = 4 * offset;                                /* column index */

    /* make sure the needed Eulerian row is computed */
    ensure_rows(R);

    /* fetch the stored Eulerian number */
    return pool[row_start[R] + k];
}
