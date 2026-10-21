/*
 * A208907  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A208756; see the Formula section.
 * https://oeis.org/A208907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   u(1,x)=1, v(1,x)=1 * u(n,x)=u(n-1,x)+2x·v(n-1,x) *
 *            v(n,x)=x·u(n-1,x)+x·v(n-1,x)+1 * a(n) = coefficient of u(row,
 *            column) when rows are written one after * another, starting with
 *            row 1. The sequence begins 1, 1, 2, 1, 4, 4, …
 * Range:     u8 n <= 41, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for memoisation of the triangle of u‑coefficients */
    static seq_t *tri_vals = NULL;   /* flattened array of all values computed so far   */
    static size_t tri_cap  = 0;      /* allocated capacity of tri_vals                 */
    static size_t tri_used = 0;      /* number of values stored in tri_vals             */

    /* state for constructing successive rows */
    static seq_t *u_prev   = NULL;   /* coefficient array of u for row r-1              */
    static seq_t *v_prev   = NULL;   /* coefficient array of v for row r-1              */
    static idx_t  prev_len = 0;      /* length of the previous row (r-1)                */

    /* initialise on first call: row 1 */
    if (tri_used == 0) {
        /* store the first coefficient 1 (row 1)  */
        tri_cap = 16;
        tri_vals = (seq_t *)malloc(tri_cap * sizeof(seq_t));
        if (!tri_vals) exit(1);            /* out‑of‑memory */
        tri_vals[0] = (seq_t)1;
        tri_used = 1;

        /* initialise u and v for the first row */
        u_prev = (seq_t *)malloc(sizeof(seq_t));
        v_prev = (seq_t *)malloc(sizeof(seq_t));
        if (!u_prev || !v_prev) exit(1);
        u_prev[0] = (seq_t)1;
        v_prev[0] = (seq_t)1;
        prev_len = 1;
    }

    /* compute rows until we have at least n values */
    while (tri_used < (size_t)n) {
        idx_t next_len = prev_len + 1;   /* the new row has one more coefficient        */

        /* allocate coefficient arrays for the new row */
        seq_t *u_next = (seq_t *)malloc(next_len * sizeof(seq_t));
        seq_t *v_next = (seq_t *)malloc(next_len * sizeof(seq_t));
        if (!u_next || !v_next) exit(1);

        /* compute coefficients of u(n,x) */
        for (idx_t j = 0; j < next_len; ++j) {
            seq_t part1 = (j < prev_len) ? u_prev[j] : (seq_t)0;
            seq_t part2 = (j > 0)          ? (seq_t)2 * v_prev[j-1] : (seq_t)0;
            u_next[j] = part1 + part2;
        }

        /* compute coefficients of v(n,x)  */
        v_next[0] = (seq_t)1;            /* +1 constant term */
        for (idx_t j = 1; j < next_len; ++j) {
            v_next[j] = u_prev[j-1] + v_prev[j-1];
        }

        /* ensure storage for the flattened triangle */
        if (tri_used + (size_t)next_len > tri_cap) {
            size_t new_cap = tri_cap ? tri_cap * 2 : 16;
            while (new_cap < tri_used + (size_t)next_len)
                new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(tri_vals, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            tri_vals = tmp;
            tri_cap  = new_cap;
        }

        /* append new row to the flattened array */
        for (idx_t j = 0; j < next_len; ++j)
            tri_vals[tri_used++] = u_next[j];

        /* free previous row arrays and adopt the new ones */
        free(u_prev);
        free(v_prev);
        u_prev   = u_next;
        v_prev   = v_next;
        prev_len = next_len;
    }

    /* return the requested term (1‑based indexing) */
    return tri_vals[n-1];
}
