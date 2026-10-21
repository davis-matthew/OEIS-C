/*
 * A210230  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A210229; see the Formula section.
 * https://oeis.org/A210230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = coefficient v(r,x) where r is the unique integer such *
 *            that r(r+1)/2 ≥ n. The value is taken from the component with *
 *            index n - r(r-1)/2 - 1 (0‑based). This follows from the
 *            triangular * layout of the coefficients of v(n,x)
 * Range:     u8 n <= 30, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* ----- internal lazy construction of the polynomial coefficients ----- */
static seq_t **u_rows = NULL;   /* pointers to rows of u(n,x) */
static seq_t **v_rows = NULL;   /* pointers to rows of v(n,x) */
static idx_t max_row = 0;       /* highest row that has been built */

/* Build rows up to 'need' (inclusive) if required. */
static void ensure_rows(idx_t need)
{
    if (need <= max_row) return;   /* nothing to do */

    /* enlarge the row pointer tables */
    u_rows = (seq_t**)realloc(u_rows, (need + 1) * sizeof(seq_t*));
    v_rows = (seq_t**)realloc(v_rows, (need + 1) * sizeof(seq_t*));
    if (!u_rows || !v_rows) exit(EXIT_FAILURE);

    /* build each new row consecutively */
    for (idx_t r = max_row + 1; r <= need; ++r) {
        idx_t prev = r - 1;
        idx_t len = r;                    /* length of the current row */

        seq_t *new_u = (seq_t*)calloc(len, sizeof(seq_t));
        seq_t *new_v = (seq_t*)calloc(len, sizeof(seq_t));
        if (!new_u || !new_v) exit(EXIT_FAILURE);

        if (r == 1) {                     /* base case */
            new_u[0] = 1;
            new_v[0] = 1;
        }
        else {
            seq_t *u_prev = u_rows[prev];
            seq_t *v_prev = v_rows[prev];

            /* u(r,0) = v(prev,0) + 1 */
            new_u[0] = v_prev[0] + 1;
            /* v(r,0) = u(prev,0) + v(prev,0) + 1 */
            new_v[0] = u_prev[0] + v_prev[0] + 1;

            for (idx_t i = 1; i < len; ++i) {
                seq_t ui_minus1 = u_prev[i-1];
                seq_t ui = (i < prev ? u_prev[i] : (seq_t)0);
                seq_t vi = (i < prev ? v_prev[i] : (seq_t)0);

                /* u(r,i) = u(prev,i-1) + v(prev,i) */
                new_u[i] = ui_minus1 + vi;
                /* v(r,i) = u(prev,i) + u(prev,i-1) + v(prev,i) */
                new_v[i] = ui + ui_minus1 + vi;
            }
        }

        /* store the freshly computed row */
        u_rows[r] = new_u;
        v_rows[r] = new_v;
    }

    max_row = need;   /* update the highest available row */
}

/* ----- public sequence function ----- */
seq_t a(idx_t n)
{
    /* Find r such that r(r+1)/2 ≥ n */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) ++r;

    /* Position within the row (0‑based, constant term first) */
    idx_t prev_total = (r - 1) * r / 2;
    idx_t pos = n - prev_total - 1;

    /* Ensure required rows are available */
    ensure_rows(r);

    /* Return the requested coefficient of v(r,x) */
    return v_rows[r][pos];
}
