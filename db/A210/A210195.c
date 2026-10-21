/*
 * A210195  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A210196; see the Formula section.
 * https://oeis.org/A210195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = flattened coefficients of the polynomial u(n,x) defined
 *            by * u(1,x)=1, v(1,x)=1 * u(n,x)=u(n-1,x)+v(n-1,x)+1 *
 *            v(n,x)=2x*u(n-1,x)+2x*v(n-1,x)+1 * The flattened list is
 *            row‑wise: first row n=1, then row n=2, etc
 * Range:     u8 n <= 24, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Cache of the coefficient arrays for all rows that have been computed. */
static seq_t **u_rows = NULL;   /* pointer to the array of u coefficients for each row */
static seq_t **v_rows = NULL;   /* pointer to the array of v coefficients for each row */
static idx_t max_rows = 0;      /* number of rows currently stored (0 means none) */

seq_t a(idx_t n)
{
    /* Ensure at least row 1 is computed. */
    if (max_rows == 0) {
        seq_t *u0 = (seq_t*)malloc(sizeof(seq_t));
        seq_t *v0 = (seq_t*)malloc(sizeof(seq_t));
        u0[0] = (seq_t)1;
        v0[0] = (seq_t)1;
        u_rows = (seq_t**)malloc(sizeof(seq_t*));
        v_rows = (seq_t**)malloc(sizeof(seq_t*));
        u_rows[0] = u0;
        v_rows[0] = v0;
        max_rows = 1;
    }

    /* Handle the first term directly. */
    if (n == 1) {
        return u_rows[0][0];
    }

    /* Find the row r that contains the n‑th term and the cumulative
       number of terms before that row (sum). */
    idx_t r = 2;
    idx_t sum = 1;                     /* terms up to row 1 */
    for ( ; ; ++r) {
        idx_t len = r - 1;             /* number of coefficients in row r */
        if (sum + len >= n) break;     /* row r contains the n‑th term */
        sum += len;
    }

    /* Compute all missing rows up to r. */
    while (max_rows < r) {
        idx_t curr = max_rows + 1;                 /* row number to compute */
        idx_t len_u = (curr == 1) ? 1 : curr - 1;   /* #(coefficients of u(curr,x)) */
        idx_t len_v = (curr == 1) ? 1 : curr;       /* #(coefficients of v(curr,x)) */

        seq_t *u = (seq_t*)malloc(len_u * sizeof(seq_t));
        seq_t *v = (seq_t*)malloc(len_v * sizeof(seq_t));

        if (curr == 1) {
            u[0] = (seq_t)1;
            v[0] = (seq_t)1;
        } else {
            /* previous row data */
            seq_t *prev_u = u_rows[curr-2];
            seq_t *prev_v = v_rows[curr-2];
            idx_t prev_u_len = (curr-1 == 1) ? 1 : (curr-1) - 1;
            idx_t prev_v_len = (curr-1 == 1) ? 1 : (curr-1);

            /* compute u(curr,x) */
            for (idx_t k = 0; k < len_u; ++k) {
                seq_t a_val = (k < prev_u_len) ? prev_u[k] : (seq_t)0;
                seq_t b_val = (k < prev_v_len) ? prev_v[k] : (seq_t)0;
                seq_t add   = (k == 0) ? (seq_t)1 : (seq_t)0;
                u[k] = a_val + b_val + add;
            }

            /* compute v(curr,x) */
            v[0] = (seq_t)1;
            for (idx_t k = 1; k < len_v; ++k) {
                seq_t a_val = (k-1 < prev_u_len) ? prev_u[k-1] : (seq_t)0;
                seq_t b_val = (k-1 < prev_v_len) ? prev_v[k-1] : (seq_t)0;
                v[k] = (seq_t)2 * (a_val + b_val);
            }
        }

        /* Append the new row to the cache. */
        u_rows = (seq_t**)realloc(u_rows, curr * sizeof(seq_t*));
        v_rows = (seq_t**)realloc(v_rows, curr * sizeof(seq_t*));
        u_rows[curr-1] = u;
        v_rows[curr-1] = v;
        ++max_rows;
    }

    /* Compute the offset within row r (0‑based). */
    idx_t offset = n - sum - 1;
    return u_rows[r-1][offset];
}
