/*
 * A208916  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A208915; see the Formula section.
 * https://oeis.org/A208916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A208916: triangle of coefficients of v(n,x) generated with
 *   u(n,x) = u(n‑1,x) + 2x*v(n‑1,x)
 *   v(n,x) = 2x*u(n‑1,x) + x*v(n‑1,x) + 1
 *   u(1,x)=1 , v(1,x)=1
 *
 * The flattened rows (row 1 first, then row 2, …) produce the sequence
 * a(1)=1, a(2)=1, a(3)=3, a(4)=1, a(5)=3, a(6)=7, …
 *
 * The function finds the row m such that a(n) lies in that row and
 * computes the required coefficient via the recurrence above.
 */

#include <stdlib.h>
#ifdef __cplusplus
#   include <vector>
#   include <algorithm>
#endif

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but safe */

    /* locate the row m : smallest m with m(m+1)/2 >= n */
    idx_t m   = 1;
    idx_t tri = 1;                 /* triangle number T_m = m(m+1)/2 */
    while (tri < n) {
        ++m;
        tri += m;                  /* T_m = T_{m‑1} + m */
    }

    idx_t prevTri = tri - m;
    idx_t col     = n - prevTri - 1;   /* 0‑based column within the row */

    size_t M = (size_t)m;           /* maximum row length needed */

    /* allocate arrays for previous and current rows */
#ifndef __cplusplus
    seq_t *u_prev = (seq_t*)calloc(M, sizeof(seq_t));
    seq_t *v_prev = (seq_t*)calloc(M, sizeof(seq_t));
    seq_t *u_curr = (seq_t*)calloc(M, sizeof(seq_t));
    seq_t *v_curr = (seq_t*)calloc(M, sizeof(seq_t));
    if (!u_prev || !v_prev || !u_curr || !v_curr) {
        free(u_prev); free(v_prev); free(u_curr); free(v_curr);
        return (seq_t)0;
    }
#else
    std::vector<seq_t> u_prev(M), v_prev(M), u_curr(M), v_curr(M);
#endif

    /* initial row (n = 1) */
#ifndef __cplusplus
    u_prev[0] = (seq_t)1;
    v_prev[0] = (seq_t)1;
#else
    u_prev[0] = 1;
    v_prev[0] = 1;
#endif

    /* iterate rows 2 .. m */
    for (size_t i = 2; i <= M; ++i) {
        size_t len_prev = i - 1;
        size_t len_curr = i;

        /* compute u_curr */
#ifndef __cplusplus
        u_curr[0] = u_prev[0];
        for (size_t k = 1; k < len_prev; ++k) {
            u_curr[k] = u_prev[k] + (seq_t)2 * v_prev[k - 1];
        }
        u_curr[len_curr - 1] = (seq_t)2 * v_prev[len_prev - 1];
#else
        u_curr[0] = u_prev[0];
        for (size_t k = 1; k < len_prev; ++k) {
            u_curr[k] = u_prev[k] + (seq_t)2 * v_prev[k - 1];
        }
        u_curr[len_curr - 1] = (seq_t)2 * v_prev[len_prev - 1];
#endif

        /* compute v_curr */
#ifndef __cplusplus
        v_curr[0] = (seq_t)1;
        for (size_t k = 1; k < len_curr; ++k) {
            v_curr[k] = (seq_t)2 * u_prev[k - 1] + v_prev[k - 1];
        }
#else
        v_curr[0] = 1;
        for (size_t k = 1; k < len_curr; ++k) {
            v_curr[k] = (seq_t)2 * u_prev[k - 1] + v_prev[k - 1];
        }
#endif

        /* swap previous and current rows */
#ifndef __cplusplus
        seq_t *tmp = u_prev; u_prev = u_curr; u_curr = tmp;
        tmp = v_prev; v_prev = v_curr; v_curr = tmp;
#else
        std::swap(u_prev, u_curr);
        std::swap(v_prev, v_curr);
#endif
    }

    /* result is the col‑th coefficient of row m */
    seq_t result;
#ifndef __cplusplus
    result = v_prev[col];
    free(u_prev); free(v_prev); free(u_curr); free(v_curr);
#else
    result = v_prev[col];
#endif

    return result;
}
