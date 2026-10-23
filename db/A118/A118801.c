/*
 * A118801  Triangle T that satisfies the matrix products: C*[T^-1]*C = T and
 *          T*[C^-1]*T = C, where C is Pascal's triangle.
 * https://oeis.org/A118801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 22, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = element T(i,j) of triangle T defined by:
 *  T(0,0) = 1,  T(n,0) = 1,
 *  T(n,k) = -T(n-1,k-1) + 2*T(n-1,k)   for n>=k>0.
 *  a(n) is obtained by flattening T row‑by‑row:
 *  let i be the unique integer with i(i+1)/2 <= n < (i+1)(i+2)/2,
 *  let j = n - i(i+1)/2; then a(n) = T(i,j).  */

/*  This routine builds rows of the triangle on demand and caches them
 *  in static storage so that repeated calls are O(1) after the first
 *  expansion up to the required row.  It works for all integer 
 *  types seq_t and idx_t defined by the build environment.  */

#include <stdlib.h>   /* for malloc, realloc */

seq_t a(idx_t n)
{
    /* Cached rows of T.  rows[i] points to an array of i+1 elements
     * holding the values T(i,0)...T(i,i).  */
    static seq_t **rows = NULL;      /* pointer to array of row pointers   */
    static idx_t  rows_count = 0;    /* number of rows currently allocated */
    static idx_t  rows_alloc = 0;    /* current capacity of rows array     */

    /* Helper: ensure rows up to index 'upto' are built.  */
    if (rows == NULL) {
        rows_alloc = 8;   /* initial capacity */
        rows = (seq_t**)malloc(rows_alloc * sizeof(seq_t*));
    }

    /* Build rows incrementally until we have row 'upto'.  */
    while (rows_count <= n) {  /* not about the index n, just a safe guard */
        idx_t i = rows_count;          /* next row number to build */
        idx_t len = i + 1;             /* number of columns in this row */

        /* Grow the rows array if necessary.  */
        if (rows_count >= rows_alloc) {
            idx_t new_alloc = rows_alloc * 2;
            seq_t **tmp = (seq_t**)realloc(rows, new_alloc * sizeof(seq_t*));
            if (tmp == NULL) return 0; /* memory failure: return 0 placeholder */
            rows = tmp;
            rows_alloc = new_alloc;
        }

        /* Allocate storage for the new row.  */
        seq_t *row = (seq_t*)malloc(len * sizeof(seq_t));
        if (row == NULL) return 0; /* memory failure */

        /* Compute the row values using the recurrence.  */
        row[0] = (seq_t)1;                /* T(i,0) = 1 */
        if (i > 0) {
            seq_t *prev = rows[i-1];      /* previous row */
            for (idx_t j = 1; j < len; ++j) {
                seq_t term1 = -(prev[j-1]);                     /* -T(i-1,j-1) */
                seq_t term2 = (j <= i-1) ? (seq_t)(2 * prev[j]) : (seq_t)0; /* 2*T(i-1,j) or 0 */
                row[j] = term1 + term2;
            }
        }

        /* Store the new row and advance.  */
        rows[i] = row;
        ++rows_count;
    }

    /* Find row i such that i(i+1)/2 <= n < (i+1)(i+2)/2.  */
    idx_t i = 0;
    idx_t cumulative = 0;
    while (1) {
        idx_t next = cumulative + (i + 1);
        if (next > n) break;
        cumulative = next;
        ++i;
    }

    idx_t j = n - cumulative;          /* column index within the row */
    return rows[i][j];                  /* return the required triangle entry */
}
