/*
 * A204205  Triangle based on (0,1/5,1) averaging array.
 * https://oeis.org/A204205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th element of the flattened triangle T(n,k)
 *            defined by T(1,1)=1, T(2,1)=1, T(2,2)=6,
 *            T(n,k)=T(n-1,k)+3*T(n-1,k-1)-2*T(n-2,k-1)-2*T(n-2,k-2) (T(n,k)=0
 *            if k<1 or k>n)
 * Range:     u8 n <= 26, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function keeps a dynamic cache of all rows that have been computed up to the
   highest requested row.  Each row is stored as an array of seq_t of length equal
   to the row index.  Memory allocation uses malloc/realloc; casts are supplied
   to satisfy both C11 and C++17. */

#include <stdlib.h>   /* malloc, realloc, free */

seq_t a(idx_t n)
{
    /* --- locate (row, col) of the n-th element ----------------------------------- */
    idx_t r = 1;
    idx_t tri = 1;                 /* r*(r+1)/2 */
    while (tri < n) {
        ++r;
        tri += r;
    }
    idx_t prev_tri = tri - r;      /* (r-1)*r/2 */
    idx_t c = n - prev_tri;        /* 1‑based column index */

    /* --- static cache of computed rows ------------------------------------------- */
    static idx_t max_row = 0;      /* highest row cached so far   */
    static seq_t **tri_cache = NULL;  /* array of pointers to rows */

    /* Allocate or resize the array of row pointers if needed. */
    if (tri_cache == NULL || r > max_row) {
        size_t new_cap = (size_t)(r + 1);          /* indices 1..r inclusive */
        seq_t **tmp = (seq_t**)realloc(tri_cache, new_cap * sizeof(seq_t*));
        if (!tmp) return (seq_t)0;                 /* allocation failure */
        tri_cache = tmp;
        /* initialise new pointers to NULL */
        for (idx_t i = max_row + 1; i <= r; ++i)
            tri_cache[i] = NULL;

        /* compute any missing rows up to r */
        for (idx_t row = max_row + 1; row <= r; ++row) {
            seq_t *row_arr = (seq_t*)malloc(sizeof(seq_t) * row);
            if (!row_arr) return (seq_t)0;         /* allocation failure */

            if (row == 1) {
                row_arr[0] = (seq_t)1;
            } else if (row == 2) {
                row_arr[0] = (seq_t)1;
                row_arr[1] = (seq_t)6;
            } else {
                seq_t *prev1 = tri_cache[row - 1];  /* row-1 values   */
                seq_t *prev2 = tri_cache[row - 2];  /* row-2 values   */
                for (idx_t k = 1; k <= row; ++k) {
                    seq_t a_val = 0, b_val = 0, c_val = 0, d_val = 0;

                    /* T(row-1, k)  */
                    if (k <= row - 1) a_val = prev1[k - 1];
                    /* T(row-1, k-1)  */
                    if (k >= 2 && k - 1 <= row - 1) b_val = prev1[k - 2];
                    /* T(row-2, k-1)  */
                    if (row >= 3 && k >= 2 && k - 1 <= row - 2) c_val = prev2[k - 2];
                    /* T(row-2, k-2)  */
                    if (row >= 3 && k >= 3 && k - 2 <= row - 2) d_val = prev2[k - 3];

                    row_arr[k - 1] = a_val + 3 * b_val - 2 * c_val - 2 * d_val;
                }
            }
            tri_cache[row] = row_arr;
        }
        max_row = r;
    }

    /* --- return the requested value --------------------------------------------- */
    seq_t *row_arr = tri_cache[r];
    return row_arr[c - 1];
}
