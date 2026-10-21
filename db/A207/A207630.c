/*
 * A207630  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A207629; see the Formula section.
 * https://oeis.org/A207630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the flattened collection of the coefficients of the
 *            polynomials v(n,x) that satisfy u(n,x) = u(n-1,x) + v(n-1,x)
 *            v(n,x) = (x+1)*u(n-1,x) + v(n-1,x) + 1 with u(1,x)=1, v(1,x)=1.
 *            The triangle read by rows has a(1)=1; a(2)=3, a(3)=1; a(4)=6,
 *            a(5)=3; a(6)=12, a(7)=9, a(8)=1; …
 * Range:     u8 n <= 20, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Handle base case n=1 directly */
    if (n == 1) return (seq_t)1;

    /* Find the row r of the triangle that contains a(n) */
    idx_t r = 1;
    idx_t cum = 0;                   // number of terms before row r
    while (cum + (r / 2 + 1) < n) {  // r/2 uses integer division
        cum += r / 2 + 1;
        r++;
    }
    /* Position within the r-th row (0‑based) */
    idx_t pos_idx = n - cum - 1;
    size_t pos = (size_t)pos_idx;

    /* Allocate temporary arrays for polynomial coefficients.
       The maximum degree needed is floor(r/2), thus arrays of size
       (r/2+2) are sufficient. */
    size_t max_len = (size_t)(r / 2) + 3;
    seq_t *u_cur = (seq_t*)malloc(sizeof(seq_t) * max_len);
    seq_t *v_cur = (seq_t*)malloc(sizeof(seq_t) * max_len);
    seq_t *u_next = (seq_t*)malloc(sizeof(seq_t) * max_len);
    seq_t *v_next = (seq_t*)malloc(sizeof(seq_t) * max_len);
    if (!u_cur || !v_cur || !u_next || !v_next) {
        /* Allocation failed; return 0 as a safe default. */
        if (u_cur) free(u_cur);
        if (v_cur) free(v_cur);
        if (u_next) free(u_next);
        if (v_next) free(v_next);
        return (seq_t)0;
    }

    /* Initial row (n = 1) */
    size_t len_u = 1;
    size_t len_v = 1;
    u_cur[0] = (seq_t)1;
    v_cur[0] = (seq_t)1;

    /* Generate rows up to r */
    for (idx_t i = 2; i <= r; ++i) {
        size_t len_u_next = (size_t)((i - 1) / 2 + 1);
        size_t len_v_next = (size_t)(i / 2 + 1);   /* equals len_u_next + 1 */

        /* Zero new coefficient arrays */
        for (size_t k = 0; k < len_u_next; ++k) u_next[k] = 0;
        for (size_t k = 0; k < len_v_next; ++k) v_next[k] = 0;

        /* new_u = u_prev + v_prev */
        for (size_t k = 0; k < len_u; ++k) u_next[k] += u_cur[k];
        for (size_t k = 0; k < len_v && k < len_u_next; ++k) u_next[k] += v_cur[k];

        /* new_v = (x+1)*u_prev + v_prev + 1 */
        for (size_t k = 0; k < len_v; ++k) v_next[k] += v_cur[k];
        for (size_t k = 0; k < len_u; ++k) v_next[k] += u_cur[k];
        for (size_t k = 0; k < len_u; ++k) v_next[k + 1] += u_cur[k];
        v_next[0] += (seq_t)1;

        /* Prepare for next iteration */
        seq_t *tmp;
        tmp = u_cur; u_cur = u_next; u_next = tmp;
        tmp = v_cur; v_cur = v_next; v_next = tmp;
        len_u = len_u_next;
        len_v = len_v_next;
    }

    /* Retrieve the required coefficient */
    seq_t value = v_cur[pos];

    /* Clean up */
    free(u_cur);
    free(v_cur);
    free(u_next);
    free(v_next);

    return value;
}
