/*
 * A209566  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A209565; see the Formula section.
 * https://oeis.org/A209566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle of coefficients of polynomials v(n,x)
 *
 * Definitions
 *   u(1,x) = 1,                     v(1,x) = 1
 *   u(n,x) = x*u(n-1,x) + v(n-1,x)
 *   v(n,x) = x*u(n-1,x) + 2x*v(n-1,x) + 1
 *
 * The sequence a(n) is obtained by listing the coefficients of
 * v(n,x) row by row:  a(1) is v(1,0), a(2) is v(2,0), a(3) is v(2,1),
 * a(4) is v(3,0), a(5) is v(3,1), a(6) is v(3,2), and so on.
 *
 * Implementation memoises coefficient vectors for all computed rows.
 * The function works for any idx_t ≥ 1 and for arbitrary‑precision
 * seq_t.  All arithmetic uses only integer operations permitted by
 * the build.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoised rows of u and v coefficient vectors.
       rows_count == number of rows already computed (>= 1 after init). */
    static size_t rows_count = 0;
    static size_t rows_cap   = 0;
    static seq_t **u_rows = NULL;  /* u_rows[i] : row i+1 coeffs, length i+1 */
    static seq_t **v_rows = NULL;  /* v_rows[i] : row i+1 coeffs */

    /* Initialise the first row if needed */
    if (rows_count == 0) {
        rows_cap = 4;  /* initial capacity */
        u_rows = (seq_t **)malloc(rows_cap * sizeof(seq_t *));
        v_rows = (seq_t **)malloc(rows_cap * sizeof(seq_t *));
        u_rows[0] = (seq_t *)malloc(sizeof(seq_t));  /* length 1 */
        v_rows[0] = (seq_t *)malloc(sizeof(seq_t));
        u_rows[0][0] = (seq_t)1;
        v_rows[0][0] = (seq_t)1;
        rows_count = 1;
    }

    /* Find the row that contains the n‑th coefficient.
       Row r has r terms, triangular index T(r) = r*(r+1)/2. */
    idx_t r = 1;
    idx_t triangular = 1;          /* T(1) */
    while (triangular < n) {
        r++;
        triangular += r;          /* now triangular = T(r) */
    }

    /* Generate rows up to r if necessary */
    while (rows_count < (size_t)r) {
        /* New row length = rows_count + 1 */
        size_t new_len = rows_count + 1;

        /* Grow pointer arrays if needed */
        if (rows_cap <= rows_count) {
            size_t new_cap = rows_cap * 2;
            seq_t **tmp_u = (seq_t **)realloc(u_rows, new_cap * sizeof(seq_t *));
            seq_t **tmp_v = (seq_t **)realloc(v_rows, new_cap * sizeof(seq_t *));
            u_rows = tmp_u;
            v_rows = tmp_v;
            rows_cap = new_cap;
        }

        /* Allocate coefficient vectors for the new row */
        seq_t *u_next = (seq_t *)malloc(new_len * sizeof(seq_t));
        seq_t *v_next = (seq_t *)malloc(new_len * sizeof(seq_t));

        /* Previous row data */
        seq_t *u_prev = u_rows[rows_count - 1];
        seq_t *v_prev = v_rows[rows_count - 1];
        size_t prev_len = rows_count;   /* length of previous row */

        /* Compute u_next */
        u_next[0] = v_prev[0];
        for (size_t k = 1; k < prev_len; ++k) {
            u_next[k] = u_prev[k - 1] + v_prev[k];
        }
        u_next[prev_len] = u_prev[prev_len - 1];

        /* Compute v_next */
        v_next[0] = (seq_t)1;
        for (size_t k = 1; k < prev_len; ++k) {
            v_next[k] = u_prev[k - 1] + (v_prev[k - 1] * (seq_t)2);
        }
        /* Last coefficient includes both u_prev and v_prev parts */
        v_next[prev_len] = u_prev[prev_len - 1] + (v_prev[prev_len - 1] * (seq_t)2);

        /* Store the new row */
        u_rows[rows_count] = u_next;
        v_rows[rows_count] = v_next;
        rows_count++;
    }

    /* Index within the selected row */
    idx_t prev_triangular = (r - 1) * r / 2;
    idx_t offset = n - prev_triangular - 1;    /* 0‑based index */

    return v_rows[r - 1][(size_t)offset];
}
