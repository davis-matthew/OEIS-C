/*
 * A209576  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A209575; see the Formula section.
 * https://oeis.org/A209576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  u(n,x)=x*u(n-1,x)+v(n-1,x),
 *               v(n,x)=2x*u(n-1,x)+2x*v(n-1,x)+1,
 *             with u(1,x)=1, v(1,x)=1.
 *
 * a(n) is the n‑th element of the triangle of coefficients of v(n,x)
 * read row‑by‑row, with the first row having one element.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* safety guard; sequence starts at n=1 */

    /* Determine the row (r) and column (c) for the n-th element. */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) ++r;          /* r such that r(r+1)/2 >= n */
    idx_t prev_terms = r * (r - 1) / 2;
    idx_t c = n - prev_terms;                   /* 1‑based column index */

    /* Special case: first row */
    if (r == 1) return (seq_t)1;

    /* Arrays for the previous row (u,v of degree r-2). */
    seq_t *U_prev = (seq_t *)malloc(sizeof(seq_t) * 1);
    seq_t *V_prev = (seq_t *)malloc(sizeof(seq_t) * 1);
    if (!U_prev || !V_prev) return 0;
    U_prev[0] = (seq_t)1;
    V_prev[0] = (seq_t)1;

    /* Build rows 2..r. */
    for (idx_t curr = 2; curr <= r; ++curr) {
        seq_t *U_cur = (seq_t *)malloc(sizeof(seq_t) * curr);
        seq_t *V_cur = (seq_t *)malloc(sizeof(seq_t) * curr);
        if (!U_cur || !V_cur) {
            free(U_prev); free(V_prev);
            return 0;
        }

        /* u_curr[j] = (j>0? u_prev[j-1]:0) + (j<curr-1? v_prev[j]:0) */
        for (idx_t j = 0; j < curr; ++j) {
            seq_t prev_U = (j > 0) ? U_prev[j - 1] : 0;
            seq_t prev_V = (j < curr - 1) ? V_prev[j] : 0;
            U_cur[j] = prev_U + prev_V;
        }

        /* v_curr[0] = 1;  v_curr[j] = 2*u_prev[j-1] + 2*v_prev[j-1] (j>0) */
        V_cur[0] = (seq_t)1;
        for (idx_t j = 1; j < curr; ++j) {
            seq_t prev_U = U_prev[j - 1];
            seq_t prev_V = V_prev[j - 1];
            V_cur[j] = prev_U * ((seq_t)2) + prev_V * ((seq_t)2);
        }

        free(U_prev);
        free(V_prev);
        U_prev = U_cur;
        V_prev = V_cur;
    }

    seq_t result = V_prev[c - 1];    /* coefficient at column c (1‑based) */
    free(U_prev);
    free(V_prev);
    return result;
}
