/*
 * A102323  Triangle, read by rows, where T(n,k) = T(n,k-1) + (2*k+1)*T(n-1,k)
 *          for n>k>0, T(n,0)=1 and T(n,n) = T(n,n-1) for n>=0.
 * https://oeis.org/A102323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row,col) where T(n,0)=1,
 *            T(n,k)=T(n,k-1)+(2*k+1)*T(n-1,k) (for n>k>0), T(n,n)=T(n,n-1).
 *            The sequence is the triangle read row by row
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Temporary storage for the rows of the triangle. */
static seq_t **rows = NULL;      /* array of pointers to rows   */
static idx_t rows_count = 0;     /* number of rows actually stored (rows[0..rows_count-1]) */
static idx_t rows_cap   = 0;     /* allocated capacity of 'rows'              */

/* Ensure that the rows array can hold at least indices 0..n (inclusive). */
static void ensure_rows_capacity(idx_t n)
{
    if (rows_cap <= n) {
        idx_t new_cap = rows_cap ? rows_cap << 1 : 1;
        if (new_cap <= n) new_cap = n + 1;
        rows = (seq_t**)realloc(rows, new_cap * sizeof(seq_t*));
        rows_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Find the row r and column k that correspond to the flattened index n. */
    idx_t r   = 0;
    idx_t cum = 0;
    while (cum + (r + 1) <= n) {
        cum += r + 1;
        r++;
    }
    idx_t k = n - cum;

    /* Initialise the first row on the first call. */
    if (rows_count == 0) {
        ensure_rows_capacity(0);
        rows[0] = (seq_t*)malloc(sizeof(seq_t));
        rows[0][0] = 1;                /* T(0,0)=1 */
        rows_count = 1;
    }

    /* Generate rows until the desired row r is available. */
    while (rows_count <= r) {
        idx_t nr  = rows_count;        /* index of the new row   */
        idx_t len = nr + 1;            /* number of elements in the new row */

        ensure_rows_capacity(nr);

        seq_t *newrow = (seq_t*)malloc(len * sizeof(seq_t));

        /* First element of the new row. */
        newrow[0] = 1;                 /* T(n,0)=1 */

        /* Interior elements. */
        seq_t *prev = rows[nr - 1];
        for (idx_t ck = 1; ck < nr; ck++) {
            seq_t term = ((seq_t)2 * ck + 1) * prev[ck];
            newrow[ck] = newrow[ck - 1] + term;
        }

        /* Last element equals the previous one. */
        newrow[nr] = newrow[nr - 1];

        /* Store the new row. */
        rows[nr] = newrow;
        rows_count++;
    }

    return rows[r][k];
}
