/*
 * A210749  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A210750; see the Formula section.
 * https://oeis.org/A210749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth term of the flattened triangle of coefficients
 *            of u(n,x), where the polynomials are defined by u(1,x)=1,
 *            v(1,x)=1 u(n,x)=(x+1)*u(n-1,x)+v(n-1,x)+1
 *            v(n,x)=(x+1)*u(n-1,x)+2x*v(n-1,x)+1 The triangle is read
 *            row‑wise, each row r containing r coefficients of u(r,x).
 *            a(1)=1, a(2)=3, a(3...
 * Range:     u8 n <= 23, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

static seq_t **u_rows = NULL;           /* pointers to the stored rows of u */
static size_t   row_cap = 0;            /* current capacity of u_rows array */
static idx_t    rows_used = 0;          /* how many rows are stored */

static seq_t *prev_u = NULL;           /* u of the last built row   */
static seq_t *prev_v = NULL;           /* v of the last built row   */

/* Allocate more room for the array of row pointers if necessary. */
static void ensure_row_cap(size_t needed) {
    if (row_cap < needed) {
        size_t new_cap = row_cap == 0 ? 16 : row_cap * 2;
        if (new_cap < needed) new_cap = needed;
        u_rows = (seq_t**)realloc(u_rows, new_cap * sizeof(seq_t*));
        row_cap = new_cap;
    }
}

/* Build the very first row (r = 1), initializing prev_u/prev_v. */
static void init_first_row(void) {
    seq_t *base_u = (seq_t*)malloc(sizeof(seq_t));
    seq_t *base_v = (seq_t*)malloc(sizeof(seq_t));
    base_u[0] = (seq_t)1;
    base_v[0] = (seq_t)1;

    ensure_row_cap(1);
    u_rows[0] = base_u;

    rows_used   = 1;
    prev_u      = base_u;
    prev_v      = base_v;
}

/* Build the next row after the current last row. */
static void build_next_row(void) {
    idx_t     r = rows_used;              /* previous row number (1‑based) */
    idx_t     len = r + 1;                /* length of the new row               */
    seq_t *next_u = (seq_t*)malloc(len * sizeof(seq_t));
    seq_t *next_v = (seq_t*)malloc(len * sizeof(seq_t));

    for (idx_t k = 0; k < len; ++k) {
        seq_t u_val = 0;
        seq_t v_val = 0;

        /* (x+1)*u_prev contributes U(k-1) and U(k) */
        if (k < r)          u_val += prev_u[k];
        if (k > 0)          u_val += prev_u[k-1];
        if (k < r)          u_val += prev_v[k];
        if (k == 0)         u_val += (seq_t)1;      /* constant term */

        /* v recurrence */
        if (k < r)          v_val += prev_u[k];
        if (k > 0)          v_val += prev_u[k-1];
        if (k > 0)          v_val += (seq_t)2 * prev_v[k-1];
        if (k == 0)         v_val += (seq_t)1;

        next_u[k] = u_val;
        next_v[k] = v_val;
    }

    /* Store the new row of u into the global table. */
    ensure_row_cap(rows_used + 1);
    u_rows[rows_used] = next_u;
    ++rows_used;

    /* Free the previous v array – its values are no longer needed. */
    free(prev_v);

    /* Update prev_u and prev_v to the freshly computed arrays. */
    prev_u = next_u;
    prev_v = next_v;
}

/* Given a desired triangle row number, build all missing rows up to it. */
static void ensure_row(idx_t r) {
    if (rows_used == 0)               /* first call – initialise base row   */
        init_first_row();

    while (rows_used < r)
        build_next_row();
}

/* Return the n-th term of the sequence (offset 1). */
seq_t a(idx_t n) {
    /* Find the row r such that T_{r-1} < n <= T_{r}, where T_{r}=r(r+1)/2 */
    idx_t row = 0;
    idx_t sum = 0;
    while (sum < n) {
        ++row;
        sum += row;
    }
    /* Now sum == T_{row}, and n lies in this row. */
    idx_t prev_sum = sum - row;          /* T_{row-1} */
    idx_t col = n - prev_sum;            /* 1‑based column index in the row */

    ensure_row(row);

    return u_rows[row-1][col-1];
}
