/*
 * A136665  Triangle of coefficients of Hermite-like analog of A053120
 *          Chebyshev's T(n, x) polynomials (powers of x in increasing order):
 *          p(x,n)=2*x*p(x,n-1)-n*p(x,n-2).
 * https://oeis.org/A136665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  uned, tabl, sign
 * Formula:   not stated
 * Range:     i8 n <= 23, i16 n <= 57, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  p(x,0)=1,  p(x,1)=x
   p(x,n)=2*x*p(x,n-1)-n*p(x,n-2).
   a(n) is the flattening of the coefficients of p(x,n), in increasing
   powers of x.  This function builds the triangle rows incrementally,
   caches the flattened sequence, and returns a(n). */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* in case a(0) is called, though n>=1 */

    /* idx is the 0‑based index into the flattened triangle */
    idx_t idx = n - 1;

    /* static cache for the flattened sequence */
    static seq_t *flat      = NULL;
    static size_t flat_len  = 0;
    static size_t flat_cap  = 0;

    /* static cache for the two most recent rows of the triangle */
    static seq_t *row_prev2 = NULL;  /* row r-2   */
    static size_t r_prev2_len = 0;
    static seq_t *row_prev1 = NULL;  /* row r-1   */
    static size_t r_prev1_len = 0;
    static idx_t current_row = 0;    /* highest row that has been built */

    /* ------------------------------------------------------ */
    /* Initialise the cache if this is the first call */
    if (flat == NULL) {
        /* row 0 : {1} */
        r_prev2_len = 1;
        row_prev2 = (seq_t *)malloc(r_prev2_len * sizeof(seq_t));
        row_prev2[0] = (seq_t)1;

        flat_cap = 1;
        flat = (seq_t *)malloc(flat_cap * sizeof(seq_t));
        flat[0] = row_prev2[0];
        flat_len = 1;

        /* row 1 : {0,1} */
        current_row = 1;
        r_prev1_len = 2;
        row_prev1 = (seq_t *)malloc(r_prev1_len * sizeof(seq_t));
        row_prev1[0] = (seq_t)0;
        row_prev1[1] = (seq_t)1;

        /* ensure enough capacity for the two rows */
        if (flat_cap < flat_len + r_prev1_len) {
            flat_cap = flat_len + r_prev1_len;
            flat = (seq_t *)realloc(flat, flat_cap * sizeof(seq_t));
        }
        flat[flat_len++] = row_prev1[0];
        flat[flat_len++] = row_prev1[1];
    }

    /* ------------------------------------------------------ */
    /* Build rows until the flattened sequence contains
       the desired index.  Each iteration constructs row r+1
       from the two previous rows. */
    while (flat_len <= (size_t)idx) {
        idx_t r = current_row + 1;       /* row number to be constructed */
        size_t len = r + 1;              /* number of coefficients in row r */

        seq_t *curr = (seq_t *)malloc(len * sizeof(seq_t));
        for (size_t k = 0; k < len; ++k) {
            seq_t from_prev1 = (k > 0 && k - 1 < r_prev1_len) ? row_prev1[k - 1] : (seq_t)0;
            seq_t from_prev2 = (k < r_prev2_len) ? row_prev2[k] : (seq_t)0;
            curr[k] = (seq_t)2 * from_prev1 - (seq_t)r * from_prev2;
        }

        /* Append the new row to the flattened cache */
        if (flat_cap < flat_len + len) {
            flat_cap = flat_len + len;
            flat = (seq_t *)realloc(flat, flat_cap * sizeof(seq_t));
        }
        for (size_t k = 0; k < len; ++k)
            flat[flat_len++] = curr[k];

        /* Shift rows: previous row1 becomes previous row2, etc. */
        free(row_prev2);
        row_prev2   = row_prev1;
        r_prev2_len = r_prev1_len;
        row_prev1   = curr;
        r_prev1_len = len;
        current_row = r;
    }

    return flat[idx];
}
