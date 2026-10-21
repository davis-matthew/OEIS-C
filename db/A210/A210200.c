/*
 * A210200  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A210199; see the Formula section.
 * https://oeis.org/A210200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth entry of the triangle of coefficients of the
 *            polynomials v(n,x) defined by: u(1,x)=1, v(1,x)=1
 *            u(n,x)=u(n-1,x)+v(n-1,x)+1 v(n,x)=(x+1)*u(n-1,x)+x*v(n-1,x)+1
 *            a(1)=v(1,0)=1, and for n>1 the entries are read row‑by‑row from
 *            v(1,x),v(2,x),v(3,x),… (row lengths 1,2,3,…)
 * Range:     u8 n <= 30, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *alloc_seq(idx_t len) {
#if defined(__cplusplus)
    return new seq_t[(size_t)len];
#else
    return (seq_t*)malloc((size_t)len * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    /* Static tables for previous rows of u and v. */
    static seq_t **u_rows = NULL;
    static seq_t **v_rows = NULL;
    static idx_t capacity = 0;   /* number of rows allocated (index 0 unused) */
    static idx_t rows_computed = 0;   /* highest row index computed */

    /* Find the row r that contains a(n): r(r+1)/2 >= n */
    idx_t r = 1;
    idx_t tri = 1;               /* number of elements up to row r */
    while (tri < n) {
        ++r;
        tri += r;
    }

    /* Grow the pointer arrays if necessary. */
    if (r >= capacity) {
        size_t new_cap = (size_t)(r + 1);   /* indices 0..r useful */
        seq_t **new_u = (seq_t**)realloc(u_rows, new_cap * sizeof(seq_t*));
        seq_t **new_v = (seq_t**)realloc(v_rows, new_cap * sizeof(seq_t*));
        if (!new_u || !new_v) {
            /* Allocation failure – return zero as a safe fallback. */
            return (seq_t)0;
        }
        u_rows = new_u;
        v_rows = new_v;
        capacity = new_cap;
    }

    /* Initialise the very first row (n = 1). */
    if (rows_computed == 0) {
        u_rows[1] = alloc_seq(1);
        v_rows[1] = alloc_seq(1);
        u_rows[1][0] = (seq_t)1;
        v_rows[1][0] = (seq_t)1;
        rows_computed = 1;
    }

    /* Compute missing rows up to r. */
    for (idx_t i = rows_computed + 1; i <= r; ++i) {
        idx_t prev_u_len = (i-1 >= 2) ? (i-2) : 1;  /* u row length of n-1 */
        idx_t prev_v_len = (i-1);                   /* v row length of n-1 */
        idx_t cur_u_len = (i >= 2) ? (i-1) : 1;     /* u row length of n   */
        idx_t cur_v_len = i;                        /* v row length of n   */

        /* Allocate new rows. */
        u_rows[i] = alloc_seq(cur_u_len);
        v_rows[i] = alloc_seq(cur_v_len);

        seq_t *prev_u = u_rows[i-1];
        seq_t *prev_v = v_rows[i-1];
        seq_t *cur_u  = u_rows[i];
        seq_t *cur_v  = v_rows[i];

        /* Compute u(i,x) = u(i-1,x)+v(i-1,x)+1. */
        cur_u[0] = prev_u[0] + prev_v[0] + (seq_t)1;
        for (idx_t j = 1; j < cur_u_len; ++j) {
            seq_t a = 0, b = 0;
            if (j < prev_u_len) a = prev_u[j];
            if (j < prev_v_len) b = prev_v[j];
            cur_u[j] = a + b;
        }

        /* Compute v(i,x) = (x+1)*u(i-1,x) + x*v(i-1,x) + 1. */
        /* Initialise all coefficients to zero. */
        for (idx_t j = 0; j < cur_v_len; ++j)
            cur_v[j] = (seq_t)0;

        /* Add (x+1)*u_prev: u_prev → coefficient 0.. and shift by 1. */
        for (idx_t j = 0; j < prev_u_len; ++j) {
            cur_v[j]     += prev_u[j];
            cur_v[j + 1] += prev_u[j];
        }

        /* Add x*v_prev: shift v_prev by 1. */
        for (idx_t j = 0; j < prev_v_len; ++j)
            cur_v[j + 1] += prev_v[j];

        /* Add constant 1 to the constant term. */
        cur_v[0] += (seq_t)1;
    }

    rows_computed = r;

    /* Compute index within the row. */
    idx_t prev_tri = ((r - 1) * r) / 2;
    idx_t k = n - prev_tri - 1;   /* zero‑based index inside row r */

    return v_rows[r][k];
}
