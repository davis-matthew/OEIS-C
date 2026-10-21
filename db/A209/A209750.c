/*
 * A209750  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A209749; see the Formula section.
 * https://oeis.org/A209750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A209750: Triangle of coefficients of polynomials v(n,x)
 * Generated from the recurrence
 *
 *    u(n,x) = x*u(n-1,x) + (x+1)*v(n-1,x)
 *    v(n,x) = u(n-1,x) + x*v(n-1,x) + 1
 *
 * with initial polynomials u(1,x)=1, v(1,x)=1.
 * The sequence a(n) is the flattening of the rows of v(n,x); i.e.,
 * a(1)=1, a(2)=2, a(3)=1, a(4)=2, a(5)=4, a(6)=1, …
 *
 * The function keeps the two polynomial coefficient arrays in static
 * storage and expands them only when a needed row is beyond what has
 * already been computed.  No large triangulated buffer is kept, so the
 * memory consumption remains modest (O(sqrt(n)) in the worst case).
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static state: the last computed row (u and v polynomials). */
    static seq_t *u = NULL;     /* u coefficients of current row */
    static seq_t *v = NULL;     /* v coefficients of current row */
    static idx_t  cur_row = 0;  /* 1‑based index of the current row */
    static idx_t  cur_size = 0; /* number of coefficients in the current row */

    /* Ensure we have the data for the row containing index n. */
    /* Compute the minimum row r such that r*(r+1)/2 >= n. */
    idx_t r = 0;
    while ((r * (r + 1) / 2) < n) ++r;

    /* Initialise on first call. */
    if (cur_row == 0) {
        cur_row  = 1;
        cur_size = 1;
        u = (seq_t *)malloc(sizeof(seq_t));
        v = (seq_t *)malloc(sizeof(seq_t));
        if (!u || !v) return 0;           /* out of memory */
        u[0] = (seq_t)1;
        v[0] = (seq_t)1;
    }

    /* Expand rows until we reach the required row r. */
    while (cur_row < r) {
        idx_t new_size = cur_size + 1;
        seq_t *new_u = (seq_t *)malloc(new_size * sizeof(seq_t));
        seq_t *new_v = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_u || !new_v) {
            free(new_u); free(new_v);
            return 0;                    /* out of memory */
        }

        /* Compute new_u[i] = x*u + (x+1)*v  (coefficients of u(n,x)) */
        for (idx_t i = 0; i < new_size; ++i) {
            seq_t val = 0;
            if (i > 0) val += u[i - 1];         /* x*u term */
            if (i < cur_size) val += v[i];      /* v term (constant part) */
            if (i > 0) val += v[i - 1];         /* x*v term */
            new_u[i] = val;
        }

        /* Compute new_v[i] = u + x*v + 1  (coefficients of v(n,x)) */
        for (idx_t i = 0; i < new_size; ++i) {
            seq_t val = 0;
            if (i < cur_size) val += u[i];       /* u term */
            if (i > 0) val += v[i - 1];          /* x*v term */
            if (i == 0) val += (seq_t)1;         /* constant 1 */
            new_v[i] = val;
        }

        /* Release old arrays and update state. */
        free(u);
        free(v);
        u = new_u;
        v = new_v;
        cur_size = new_size;
        ++cur_row;
    }

    /* Determine the offset of n within the current row. */
    idx_t prev_tri = (cur_row - 1) * cur_row / 2;  /* total elements before this row */
    idx_t offset   = n - prev_tri - 1;             /* zero‑based index in current row */

    return v[offset];
}
