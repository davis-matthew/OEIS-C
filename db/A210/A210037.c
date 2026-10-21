/*
 * A210037  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A210038; see the Formula section.
 * https://oeis.org/A210037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n‑th term in the flattened triangle of the
 *            coefficients of the polynomials u(n,x) defined by u(1,x)=1,
 *            v(1,x)=1 u(n,x)=u(n-1,x)+v(n-1,x)+1
 *            v(n,x)=u(n-1,x)+(x+1)*v(n-1,x)+1 The term a(1)=1, a(2)=3,
 *            a(3)=7, a(4)=1, ... as given in A210037
 * Range:     u8 n <= 23, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is constructed by storing the vectors of coefficients
   u(1,x), u(2,x), … in a static cache.  Each row r has length
   (r==1?1:r-1).  If n is larger than the current cached row, the
   cache is expanded until the desired row is computed.  The function
   then returns the k‑th coefficient of that row, where k is the
   offset of n inside the row.  This works for any idx_t/seq_t
   combination that satisfies the requirements. */

#include <stdlib.h>

static idx_t max_row = 0;          /* highest row that has been computed */
static seq_t **u_rows = NULL;      /* u_rows[r] points to coefficients of u(r,x) */
static seq_t **v_rows = NULL;      /* v_rows[r] points to coefficients of v(r,x) */

/* Expand the row cache up to row r.  All rows are 1‑based. */
static void ensure_rows(idx_t r)
{
    if (r <= max_row) return;

    /* Grow the row pointers. */
    u_rows = (seq_t**)realloc(u_rows, sizeof(seq_t*) * (r + 1));
    v_rows = (seq_t**)realloc(v_rows, sizeof(seq_t*) * (r + 1));

    /* Compute rows one by one. */
    for (idx_t row = max_row + 1; row <= r; ++row)
    {
        size_t u_len = (row == 1 ? 1 : row - 1);
        size_t v_len = (row == 1 ? 1 : row);

        seq_t *u = (seq_t*)calloc(u_len, sizeof(seq_t));
        seq_t *v = (seq_t*)calloc(v_len, sizeof(seq_t));

        if (row == 1) {
            u[0] = 1;
            v[0] = 1;
        } else {
            seq_t *u_prev = u_rows[row - 1];
            seq_t *v_prev = v_rows[row - 1];
            size_t u_prev_len = (row - 1 == 1 ? 1 : (row - 1) - 1);
            size_t v_prev_len = (row - 1 == 1 ? 1 : (row - 1));

            /* u(row) = u(row-1) + v(row-1) + 1 (constant polynomial) */
            for (size_t k = 0; k < u_len; ++k) {
                seq_t val = 0;
                if (k < u_prev_len) val += u_prev[k];
                if (k < v_prev_len) val += v_prev[k];
                if (k == 0)          val += 1;        /* constant term */
                u[k] = val;
            }

            /* v(row) = u(row-1) + v(row-1) + x*v(row-1) + 1 */
            for (size_t k = 0; k < v_len; ++k) {
                seq_t val = 0;
                if (k < u_prev_len)          val += u_prev[k];
                if (k < v_prev_len)          val += v_prev[k];
                if (k > 0 && k - 1 < v_prev_len)  val += v_prev[k - 1]; /* x*term */
                if (k == 0)                  val += 1;                    /* constant */
                v[k] = val;
            }
        }

        u_rows[row] = u;
        v_rows[row] = v;
    }

    max_row = r;
}

/* Return the n‑th term of sequence A210037 (1‑based). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;      /* undefined for n < 1, but not expected */

    /* Locate the row that contains the n‑th term. */
    idx_t row = 1;
    idx_t cum = 0;                /* number of terms before this row */
    while (1) {
        size_t terms_in_row = (row == 1 ? 1 : row - 1);
        if (cum + terms_in_row >= n) break;
        cum += (idx_t)terms_in_row;
        ++row;
    }

    ensure_rows(row);            /* compute necessary rows */

    idx_t k = n - cum - 1;        /* 0‑based index within the row */
    return u_rows[row][k];
}
