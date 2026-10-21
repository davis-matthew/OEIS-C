/*
 * A210202  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A210201; see the Formula section.
 * https://oeis.org/A210202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the coefficient of x^k in the polynomial v(m,x) where m
 *            satisfies T(m-1) < n ≤ T(m) and k = n - T(m-1) - 1. The
 *            polynomials are generated recursively by: u(1,x)=1, v(1,x)=1
 *            u(n,x)=u(n-1,x)+v(n-1,x)+1 v(n,x)=(x+1)*u(n-1,x)+2x*v(n-1,x)+1
 *            The sequence a is the triangle of coefficients of v(n...
 * Range:     u8 n <= 23, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static storage for the rows of u and v polynomials. */
static idx_t  max_row   = 0;     /* highest row built so far */
static seq_t **u_rows   = NULL;  /* pointers to coefficient arrays of u */
static seq_t **v_rows   = NULL;  /* pointers to coefficient arrays of v */
static size_t  rows_cap = 0;     /* capacity of the pointer arrays */

/* Ensure the pointer arrays have capacity at least 'needed'. */
static void extend_cap(size_t needed)
{
    if (rows_cap >= needed) return;
    size_t new_cap = rows_cap ? rows_cap : 16;
    while (new_cap < needed) new_cap <<= 1;
    u_rows = (seq_t **)realloc(u_rows, new_cap * sizeof(seq_t *));
    v_rows = (seq_t **)realloc(v_rows, new_cap * sizeof(seq_t *));
    rows_cap = new_cap;
}

/* Build all rows up to and including 'up_to'. */
static void ensure_rows(idx_t up_to)
{
    if (up_to <= max_row) return;

    if (max_row == 0) {
        /* Build row 1 */
        extend_cap(1);
        u_rows[0] = (seq_t *)calloc(1, sizeof(seq_t));
        v_rows[0] = (seq_t *)calloc(1, sizeof(seq_t));
        u_rows[0][0] = (seq_t)1;
        v_rows[0][0] = (seq_t)1;
        max_row = 1;
    }

    for (idx_t r = max_row + 1; r <= up_to; ++r) {
        if (rows_cap < up_to) extend_cap(up_to);

        /* Sizes for previous row (r-1) */
        idx_t prev_u_size = (r == 2 ? 1 : r - 2);   /* u(r-1) size */
        idx_t prev_v_size = r - 1;                 /* v(r-1) size */

        /* Sizes for current row r */
        idx_t u_size = (r == 1 ? 1 : r - 1);       /* u(r) size  */
        idx_t v_size = r;                          /* v(r) size  */

        /* Allocate new rows */
        seq_t *u_new = (seq_t *)calloc(u_size, sizeof(seq_t));
        seq_t *v_new = (seq_t *)calloc(v_size, sizeof(seq_t));

        if (r == 1) {
            u_new[0] = (seq_t)1;
            v_new[0] = (seq_t)1;
        } else {
            seq_t *u_prev = u_rows[r - 2];
            seq_t *v_prev = v_rows[r - 2];

            /* Compute u(r,x) = u(prev) + v(prev) + 1 (only constant term gets +1) */
            u_new[0] = u_prev[0] + v_prev[0] + (seq_t)1;
            for (idx_t k = 1; k < u_size; ++k) {
                seq_t up = (k < prev_u_size) ? u_prev[k] : 0;
                seq_t vp = (k < prev_v_size) ? v_prev[k] : 0;
                u_new[k] = up + vp;
            }

            /* Compute v(r,x) = (x+1)*u(prev) + 2x*v(prev) + 1 */
            v_new[0] = u_prev[0] + (seq_t)1;
            for (idx_t k = 1; k < v_size; ++k) {
                seq_t term1 = (k < prev_u_size) ? u_prev[k] : 0;
                seq_t term2 = (k - 1 < prev_u_size) ? u_prev[k - 1] : 0;
                seq_t term3 = (seq_t)2 * v_prev[k - 1];
                v_new[k] = term1 + term2 + term3;
            }
        }

        u_rows[r - 1] = u_new;
        v_rows[r - 1] = v_new;
    }

    max_row = up_to;
}

/* Return the n-th element of the flattened triangle (1‑based). */
seq_t a(idx_t n)
{
    /* Find the row m that contains the n-th coefficient. */
    idx_t accum = 0, m = 1;
    while (accum + m < n) {
        accum += m;
        ++m;
    }

    /* Ensure that row m is built. */
    ensure_rows(m);

    /* Index within the row (0‑based). */
    idx_t k = n - accum - 1;

    return v_rows[m - 1][(size_t)k];
}
