/*
 * A102365  Triangle T(n,k), 0 <= k <= n, read by rows: given by [ 1, 0, 3, 0,
 *          5, 0, 7, 0, 9, 0, ...] DELTA [ 0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 0,
 *          ...] where DELTA is the operator defined in A084938.
 * https://oeis.org/A102365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r,c) where T(n,k) satisfies T(0,0)=1, T(n,k)=0 for k<0
 *            or k>n, T(n,k) = (n-k)*T(n-1,k-1) + (2*k+1)*T(n-1,k)
 * Range:     u8 n <= 22, u16 n <= 38, u32 n <= 94, u64 n <= 215,
 *            u128 n <= 539, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

static seq_t **rows = NULL;     /* pointers to each triangle row */
static idx_t  num_rows = 0;     /* number of rows currently stored (next index) */
static idx_t  rows_capacity = 0;/* allocated capacity of rows array */

/* Ensure the triangle contains rows 0..r */
static void ensure_rows(idx_t r)
{
    if (r < num_rows) return;

    /* Increase capacity if needed */
    if (rows_capacity == 0) {
        rows_capacity = 4;
        rows = (seq_t**)malloc(rows_capacity * sizeof(seq_t*));
    }
    if (rows_capacity <= r) {
        /* grow capacity */
        idx_t new_capacity = rows_capacity;
        while (new_capacity <= r) new_capacity *= 2;
        seq_t **new_rows = (seq_t**)realloc(rows, new_capacity * sizeof(seq_t*));
        rows = new_rows;
        rows_capacity = new_capacity;
    }

    /* Build missing rows */
    for (; num_rows <= r; ++num_rows) {
        idx_t size = num_rows + 1;
        seq_t *row = (seq_t*)malloc(size * sizeof(seq_t));
        if (!row) {
            /* memory failure – abort; sequence value undefined */
            exit(1);
        }

        if (num_rows == 0) {
            /* row 0 */
            row[0] = 1;
        } else {
            seq_t *prev = rows[num_rows-1];
            row[0] = 1;
            for (idx_t k = 1; k < num_rows; ++k) {
                seq_t term1 = (seq_t)(num_rows - k) * prev[k-1];
                seq_t term2 = (seq_t)(2*k + 1) * prev[k];
                row[k] = term1 + term2;
            }
            row[num_rows] = 0; /* last entry is always 0 */
        }
        rows[num_rows] = row;
    }
}

/* Compute the triangle entry T(n,k) given linear index n */
seq_t a(idx_t n)
{
    /* find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    double d = sqrt((double)8 * (double)n + 1.0);
    idx_t r = (idx_t)((d - 1.0) / 2.0);
    idx_t t = r * (r + 1) / 2;   /* number of entries before row r */
    idx_t c = n - t;             /* column within the row */

    ensure_rows(r);
    return rows[r][c];
}
