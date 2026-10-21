/*
 * A096591  Triangle T, read by rows, such that the matrix square shifts T one
 *          place diagonally left and upward, with T(n,0)=T(n,n)=1 for n>=0.
 * https://oeis.org/A096591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the triangle T by rows (row 0: 1,
 *            row 1: 1,1, row 2: 1,1,1, …), where T(n,0)=T(n,n)=1 for n≥0, and
 *            for n>1 and 0<k<n T(n,k) = Σ_{i=0}^{n-1} T(n-2,i) * T(i,k-1)
 * Range:     u8 n <= 56, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    /* static caches of previously computed rows */
    static seq_t **rows = NULL;          /* array of pointers to rows */
    static size_t rows_count   = 0;      /* number of rows allocated   */
    static size_t rows_allocated = 0;    /* current capacity of rows    */

    /* 1. Find the row that contains the index n.
       Row r starts at offset r(r+1)/2.  */
    size_t r = 0;
    while (((idx_t)(r + 1) * (r + 2)) / 2 <= n)
        ++r;   /* safe: r grows like sqrt(2n)            */

    /* 2. Ensure rows 0..r are computed. */
    while (rows_count <= r) {
        /* Allocate new row of length row+1.  */
        seq_t *cur = (seq_t *)malloc((rows_count + 1) * sizeof(seq_t));
        if (!cur) abort();          /* out of memory */

        /* Boundary values are always 1.  */
        cur[0] = cur[rows_count] = (seq_t)1;

        /* Compute inner elements for rows with index ≥ 2.  */
        if (rows_count >= 2) {
            for (size_t k = 1; k < rows_count; ++k) {
                seq_t sum = (seq_t)0;

                /* i runs from k-1 to rows_count-2 (inclusive)  
                   because T(i,k-1)=0 when i<k-1 and
                   T(rows_count-2,i)=0 when i>rows_count-2.      */
                for (size_t i = k - 1; i <= rows_count - 2; ++i) {
                    sum += rows[rows_count - 2][i] * rows[i][k - 1];
                }
                cur[k] = sum;
            }
        }

        /* Grow the rows array if necessary.  */
        if (rows_count >= rows_allocated) {
            size_t new_alloc = rows_allocated ? rows_allocated * 2 : 4;
            seq_t **tmp = (seq_t **)realloc(rows, new_alloc * sizeof(seq_t *));
            if (!tmp) { free(cur); abort(); }
            rows = tmp;
            rows_allocated = new_alloc;
        }

        /* Store the computed row.  */
        rows[rows_count++] = cur;
    }

    /* 3. Locate the element a(n) within the found row.  */
    idx_t offset = (idx_t)r * (r + 1) / 2;      /* first index of row r */
    return rows[r][n - offset];
}
