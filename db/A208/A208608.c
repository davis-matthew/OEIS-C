/*
 * A208608  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A208609; see the Formula section.
 * https://oeis.org/A208608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A208608:  Triangle of coefficients of polynomials u(n,x).
   The flattened coefficients are returned in order of the triangle
   rows.  */
#include <stdlib.h>

/*  The build provides the types seq_t and idx_t.  */

/*  Transform a global index n into the coefficient of u(nrow,x) at
    position pos within that row.  */
seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 >= n.  */
    idx_t r = 1;
    idx_t tri = 1;
    while (tri < n) {
        ++r;
        tri += r;
    }
    /* prev_tri = T(r-1) */
    idx_t prev_tri = tri - r;
    /* position within the row (0‑based) */
    idx_t pos = n - prev_tri - 1;

    /* Ensure that all rows up to r are computed.  */
    static seq_t **u_rows = NULL;        /* pointers to u(n,x) rows */
    static seq_t **v_rows = NULL;        /* pointers to v(n,x) rows */
    static size_t *row_sizes = NULL;     /* length of each row */
    static idx_t max_row = 0;            /* highest row already computed */
    static size_t cap_rows = 0;          /* allocated capacity for row pointers */

    if (r > max_row) {
        /* Grow the array of row pointers if needed.  */
        if (cap_rows == 0) {
            cap_rows = 8;
            u_rows = (seq_t**)malloc(cap_rows * sizeof(seq_t*));
            v_rows = (seq_t**)malloc(cap_rows * sizeof(seq_t*));
            row_sizes = (size_t*)malloc(cap_rows * sizeof(size_t));
        } else if ((size_t)max_row + 1 > cap_rows) {
            cap_rows = cap_rows * 2;
            u_rows = (seq_t**)realloc(u_rows, cap_rows * sizeof(seq_t*));
            v_rows = (seq_t**)realloc(v_rows, cap_rows * sizeof(seq_t*));
            row_sizes = (size_t*)realloc(row_sizes, cap_rows * sizeof(size_t));
        }

        /* Compute new rows from max_row+1 up to r.  */
        for (idx_t i = max_row + 1; i <= r; ++i) {
            size_t len = (size_t)i;                /* number of coefficients */
            seq_t *u_new = (seq_t*)malloc(len * sizeof(seq_t));
            seq_t *v_new = (seq_t*)malloc(len * sizeof(seq_t));
            if (i == 1) {
                /* u(1,x) = 1,  v(1,x) = 1  */
                u_new[0] = (seq_t)1;
                v_new[0] = (seq_t)1;
            } else {
                /* previous row */
                seq_t *u_prev = u_rows[i - 2];
                seq_t *v_prev = v_rows[i - 2];
                size_t prev_len = len - 1;

                /* u(i,x) = u(i-1,x) + x * v(i-1,x) */
                u_new[0] = u_prev[0];
                for (size_t k = 1; k < prev_len; ++k)
                    u_new[k] = u_prev[k] + v_prev[k - 1];
                u_new[prev_len] = v_prev[prev_len - 1];

                /* v(i,x) = (x+1) * u(i-1,x) + x * v(i-1,x) + 1 */
                v_new[0] = u_prev[0] + (seq_t)1;
                for (size_t k = 1; k < prev_len; ++k)
                    v_new[k] = u_prev[k] + u_prev[k - 1] + v_prev[k - 1];
                v_new[prev_len] = u_prev[prev_len - 1] + v_prev[prev_len - 1];
            }
            u_rows[i - 1] = u_new;
            v_rows[i - 1] = v_new;
            row_sizes[i - 1] = len;
        }
        max_row = r;
    }

    /* Return the requested coefficient.  */
    return u_rows[r - 1][pos];
}
