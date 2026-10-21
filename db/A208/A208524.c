/*
 * A208524  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A208525; see the Formula section.
 * https://oeis.org/A208524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   u(1,x)=1, v(1,x)=1; u(n,x)=u(n-1,x)+x*v(n-1,x),
 *            v(n,x)=2x*u(n-1,x)+(x+1)*v(n-1,x)+1. a(n) = coefficient of
 *            u(row,r) at degree offset, where rows are flattened: row 1 has 1
 *            coefficient, row 2 has 2, etc
 * Range:     u8 n <= 31, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the nth term of the triangle read row by row. */

#include <stdlib.h>

extern seq_t a(idx_t n);

seq_t a(idx_t n) {
    /* find row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    idx_t tri = 1;           /* T(r) */
    while (tri < n) {
        ++r;
        tri += r;
    }
    idx_t prev_tri = tri - r;      /* T(r-1) */
    idx_t offset = n - prev_tri - (idx_t)1;   /* 0‑based index in row r */

    /* allocate arrays for coefficients of u and v */
    seq_t *u   = (seq_t*)calloc(r, sizeof(seq_t));
    seq_t *v   = (seq_t*)calloc(r, sizeof(seq_t));
    seq_t *u2  = (seq_t*)calloc(r, sizeof(seq_t));
    seq_t *v2  = (seq_t*)calloc(r, sizeof(seq_t));
    if (!u || !v || !u2 || !v2) {
        /* allocation failed; return zero as fallback */
        if(u) free(u); if(v) free(v); if(u2) free(u2); if(v2) free(v2);
        return (seq_t)0;
    }

    /* initial polynomials for n = 1 */
    u[0] = (seq_t)1;
    v[0] = (seq_t)1;

    /* iterate to compute rows up to r */
    for (idx_t k = 2; k <= r; ++k) {
        /* compute u2, v2 for row k using u, v from row k-1 */
        for (idx_t d = 0; d < k; ++d) {
            /* u(k,x) = u(k-1,x) + x * v(k-1,x) */
            u2[d] = u[d];
            if (d > 0) u2[d] += v[d - 1];

            /* v(k,x) = 2x * u(k-1,x) + (x+1) * v(k-1,x) + 1 */
            seq_t t1 = 0;
            if (d > 0) t1 = u[d - 1] * (seq_t)2;

            seq_t t2 = v[d];
            if (d > 0) t2 += v[d - 1];

            seq_t t3 = (d == 0) ? (seq_t)1 : (seq_t)0;

            v2[d] = t1 + t2 + t3;
        }
        /* swap buffers for next iteration */
        seq_t *tmp = u;  u = u2;  u2 = tmp;
        tmp = v;        v = v2;  v2 = tmp;
    }

    seq_t result = u[offset];

    /* free all allocated memory */
    free(u);    /* after swaps, original pointers are freed */
    free(u2);
    free(v);
    free(v2);

    return result;
}
