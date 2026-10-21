/*
 * A208757  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A208758; see the Formula section.
 * https://oeis.org/A208757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle of coefficients of u(n,x).  The term a(n) is read row‑by‑row from the triangle. */

#include <stdlib.h>

extern seq_t a(idx_t n);

/* Internal storage of the computed rows of the triangle.  */
static seq_t **u_rows = NULL;     /* u_rows[i] points to array of length i+1 storing row i coefficients of u. */
static idx_t   cached_rows = 0;   /* Number of rows that have already been computed. */

static seq_t *v_curr = NULL;     /* Current v array (coefficients of v for the last computed row). */
static idx_t  v_len   = 0;        /* Length of v_curr. */

/* Expand the triangle to at least include row r_needed (0‑based). */
static void
ensure_rows(idx_t r_needed)
{
    while (cached_rows <= r_needed) {
        idx_t          r     = cached_rows;
        size_t         len   = (size_t)r + 1;
        seq_t          *row   = (seq_t*)calloc(len, sizeof(seq_t));
        if (!row) return;                     /* Allocation failure; caller will get indeterminate result */

        if (r == 0) {                         /* First row: u(1,x) = 1,  v(1,x) = 1 */
            row[0] = (seq_t)1;
            u_rows = (seq_t**)realloc(u_rows, sizeof(seq_t*));
            u_rows[0] = row;

            /* Allocate a separate v array for the first row. */
            v_curr = (seq_t*)malloc(sizeof(seq_t));
            if (!v_curr) return;
            v_curr[0] = (seq_t)1;
            v_len = 1;
        } else {
            /* Prepare to compute the next row from the previous one. */
            seq_t *u_prev = u_rows[r-1];
            size_t prev_len = r;          /* Length of the previous row */
            seq_t *v_prev = v_curr;        /* v array for the previous row */

            /* Compute u_next and v_next. */
            seq_t *u_next = row;
            seq_t *v_next = (seq_t*)calloc(len, sizeof(seq_t));
            if (!v_next) {
                free(row);
                return;
            }

            for (size_t j = 0; j < len; ++j) {
                seq_t val  = 0;
                if (j < prev_len) val += u_prev[j];
                if (j > 0)       val += (seq_t)2 * v_prev[j-1];
                u_next[j] = val;

                seq_t v_val = 0;
                if (j > 0) {
                    v_val += u_prev[j-1];
                    v_val += (seq_t)2 * v_prev[j-1];
                }
                v_next[j] = v_val;
            }

            /* Store the new row and replace the current v array. */
            u_rows = (seq_t**)realloc(u_rows, (r+1) * sizeof(seq_t*));
            u_rows[r] = u_next;

            free(v_curr);
            v_curr = v_next;
            v_len  = len;
        }

        ++cached_rows;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;                 /* Undefined for n < 1 */

    /* Determine the row r (0‑based) that contains index n.  */
    idx_t r = 0;
    while (((idx_t)(r + 1) * (r + 2) / 2) < n) ++r;

    /* Compute the triangle up to this row if necessary. */
    if (cached_rows <= r)
        ensure_rows(r);

    /* Column index c within the row. */
    idx_t prev_total = r * (r + 1) / 2;          /* Total entries before row r */
    idx_t c = n - prev_total - 1;                /* 0‑based column */

    return u_rows[r][c];
}
