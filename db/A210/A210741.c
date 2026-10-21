/*
 * A210741  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A210742; see the Formula section.
 * https://oeis.org/A210741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence enumerates all coefficients of the polynomials u(n,x) (A210741).
 *
 * The polynomials are defined by
 *   u(1,x) = 1 ,  v(1,x) = 1
 *   u(n,x) = 2x·u(n-1,x) + x·v(n-1,x) + 1
 *   v(n,x) = (x+1)·u(n-1,x)+(x+1)·v(n-1,x) + 1   for n>1
 *
 * a(n) is the n-th coefficient in row‑major order of the triangular
 * array whose rows are the coefficient lists of u(1,x), u(2,x), u(3,x), …
 * The function keeps the already computed rows in static storage so that
 * repeated calls are fast and calls may arrive in any order.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static data shared between calls */
    static seq_t *seq = NULL;      /* flattened list of all coefficients of u */
    static idx_t seq_len = 0;      /* number of coefficients stored so far */
    static idx_t max_row = 0;      /* highest row that has been computed */
    static seq_t *u_prev = NULL;   /* coefficients of u(max_row, x) */
    static seq_t *v_prev = NULL;   /* coefficients of v(max_row, x) */

    if (n < 1) return 0;   /* defensive guard; the caller guarantees n>=1 */

    /* --- initialise for the first row, if this is the first call --- */
    if (seq_len == 0) {
        u_prev = (seq_t *)malloc(sizeof(seq_t));
        v_prev = (seq_t *)malloc(sizeof(seq_t));
        u_prev[0] = 1;          /* u(1,0) */
        v_prev[0] = 1;          /* v(1,0) */

        seq = (seq_t *)malloc(sizeof(seq_t));
        seq[0] = 1;             /* first coefficient of u(1,x) */
        seq_len = 1;
        max_row = 1;
    }

    /* --- grow the triangle until we have at least n coefficients --- */
    while (seq_len < n) {
        idx_t row = max_row + 1;            /* new row number (1‑based) */

        /* allocate arrays for the new row */
        seq_t *u_curr = (seq_t *)malloc(row * sizeof(seq_t));
        seq_t *v_curr = (seq_t *)malloc(row * sizeof(seq_t));

        /* constant term of u(n,x) is always 1 */
        u_curr[0] = 1;

        /* v(n,x) constant term */
        v_curr[0] = u_prev[0] + v_prev[0] + 1;

        /* compute remaining coefficients for k = 1 .. row-1 */
        for (idx_t k = 1; k < row; ++k) {
            /* coefficients of u(n,x) depend only on v_prev[k-1] and u_prev[k-1] */
            seq_t upk_1 = u_prev[k - 1];
            seq_t vp_1  = v_prev[k - 1];
            u_curr[k] = (upk_1 * 2) + vp_1;   /* 2*u_prev[k-1] + v_prev[k-1] */

            /* coefficients of v(n,x) need u_prev[k], v_prev[k], upk_1, vp_1 */
            seq_t upk   = (k < row - 1) ? u_prev[k] : 0;
            seq_t vp    = (k < row - 1) ? v_prev[k] : 0;
            v_curr[k] = upk + vp + upk_1 + vp_1;
        }

        /* append u_curr coefficients to the flattened sequence */
        seq = (seq_t *)realloc(seq, (seq_len + row) * sizeof(seq_t));
        for (idx_t k = 0; k < row; ++k) {
            seq[seq_len + k] = u_curr[k];
        }
        seq_len += row;

        /* free the previous row and promote the current row to 'previous' */
        free(u_prev);
        free(v_prev);
        u_prev = u_curr;
        v_prev = v_curr;
        max_row = row;
    }

    /* seq is 0‑based, n is 1‑based */
    return seq[n - 1];
}
