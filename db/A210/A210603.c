/*
 * A210603  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A210738; see the Formula section.
 * https://oeis.org/A210603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Triangle of coefficients of polynomials u(n,x) defined by
 *            u(1,x)=1, v(1,x)=1 u(n,x)=2x*u(n-1,x)+v(n-1,x)+1
 *            v(n,x)=(x+1)*u(n-1,x)+(x+1)*v(n-1,x)+1 The sequence a(n) is
 *            obtained by flattening the rows of u(n,x) (row n has n
 *            coefficients). a(n) is the n-th coefficient in this flattened
 *            triangle
 * Range:     u8 n <= 23, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note:  The code uses malloc/realloc and free.  Because the
 * build might use a class type for seq_t, the returned pointers are
 * cast explicitly to the appropriate type to satisfy C++ without
 * implicit conversion errors. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find the row r that contains the n-th flattened term.
       Row r has r coefficients and the total number of terms up
       to row r-1 is T_{r-1}=r*(r-1)/2. */
    idx_t r   = 1;      /* candidate row number (1‑based) */
    idx_t t   = 0;      /* total terms before the current row (T_{r-1}) */

    while ((t + r) < n)            /* if n is beyond this row, move to next */
    {
        ++r;
        t += r - 1;                /* t becomes T_{r-1} after increment */
    }

    idx_t k = n - t - 1;           /* zero‑based position inside the row */

    /* Static storage for computed rows.  triangle[r-1] points to the
       array of u(n,x) coefficients for row r.  We keep all rows in
       memory so that calls with arbitrary n do not need to recompute
       from scratch. */
    static seq_t **triangle   = NULL;   /* array of pointers to row buffers */
    static size_t rows_alloc  = 0;      /* current capacity of triangle[]    */
    static size_t n_rows_done = 0;      /* number of rows that have been built */

    static seq_t *prev_u = NULL;       /* u coefficients of previous row   */
    static seq_t *prev_v = NULL;       /* v coefficients of previous row   */

    /* Expand the triangle pointer array if required. */
    if ((size_t)r > rows_alloc)
    {
        size_t new_size = (size_t)r;
        seq_t **tmp = (seq_t **)realloc(triangle, new_size * sizeof(seq_t *));
        if (!tmp) exit(1);            /* allocation failure */
        triangle = tmp;
        rows_alloc = new_size;
    }

    /* Build rows up to r if not already present. */
    while (n_rows_done < (size_t)r)
    {
        /* Allocate buffers for the new row. */
        size_t row_len = ++n_rows_done;               /* length of this row (1‑based) */
        seq_t *u_row = (seq_t *)malloc(row_len * sizeof(seq_t));
        seq_t *v_row = (seq_t *)malloc(row_len * sizeof(seq_t));
        if (!u_row || !v_row) exit(1);                /* allocation failure */

        if (n_rows_done == 1)      /* base case: u(1,x)=v(1,x)=1 */
        {
            u_row[0] = (seq_t)1;
            v_row[0] = (seq_t)1;
        }
        else
        {
            size_t prev_len = n_rows_done - 1;  /* previous row length */
            for (size_t i = 0; i < row_len; ++i)
            {
                /* u(n,i) = 2*u_{i-1} + v_i + (i==0?1:0) */
                seq_t u_val = (i > 0 ? (seq_t)2 * prev_u[i-1] : (seq_t)0);
                u_val += (i < prev_len ? prev_v[i] : (seq_t)0);
                u_val += (i == 0 ? (seq_t)1 : (seq_t)0);
                u_row[i] = u_val;

                /* v(n,i) = u_{i-1} + u_i + v_{i-1} + v_i + (i==0?1:0) */
                seq_t v_val = (i > 0 ? prev_u[i-1] : (seq_t)0);
                v_val += (i < prev_len ? prev_u[i] : (seq_t)0);
                v_val += (i > 0 ? prev_v[i-1] : (seq_t)0);
                v_val += (i < prev_len ? prev_v[i] : (seq_t)0);
                v_val += (i == 0 ? (seq_t)1 : (seq_t)0);
                v_row[i] = v_val;
            }
        }

        /* Store the u coefficients for this row. */
        triangle[n_rows_done - 1] = u_row;

        /* Free the previous v buffer (no longer needed). */
        free(prev_v);
        /* Update the pointers for the next iteration. */
        prev_u = u_row;
        prev_v = v_row;
    }

    /* Return the requested coefficient from the stored row. */
    return triangle[r-1][k];
}
