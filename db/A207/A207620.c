/*
 * A207620  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A207621; see the Formula section.
 * https://oeis.org/A207620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   * a(n) is the coefficient of x^(c-1) in the polynomial u(r,x),
 *            where r is * the unique integer ≥1 with 1 + r(r-1)/2 ≥ n. The
 *            polynomials are * defined by * * u(1,x) = 1, * v(1,x) = 1, *
 *            u(n,x) = u(n-1,x) + v(n-1,x), * v(n,x) = x*u(n-1,x) +
 *            (x+1)*v(n-1,x) + 1. * * The triangle of coefficients is r...
 * Range:     u8 n <= 24, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first term separately. */
    if (n == 1)
        return (seq_t)1;

    /* Determine the row r such that 1 + r(r-1)/2 >= n. */
    idx_t r = 1;
    while (1 + r * (r - 1) / 2 < n)
        ++r;

    /* Compute the starting index of row r. */
    idx_t start = 2 + (r - 1) * (r - 2) / 2;   /* for r >= 2 */
    idx_t col   = n - start;                  /* 0‑based column */

    /* Allocate temporary buffers.  Size r+1 is safe for all needed arrays. */
    seq_t *u     = (seq_t *)calloc(r + 1, sizeof(seq_t));
    seq_t *v     = (seq_t *)calloc(r + 1, sizeof(seq_t));
    seq_t *u_new = (seq_t *)calloc(r + 1, sizeof(seq_t));
    seq_t *v_new = (seq_t *)calloc(r + 1, sizeof(seq_t));
    if (!u || !v || !u_new || !v_new) {
        /* Allocation failure – return 0 as a safe fallback. */
        free(u); free(v); free(u_new); free(v_new);
        return (seq_t)0;
    }

    /* Initial polynomials for n = 1. */
    idx_t deg_u = 1;          /* current degree of u (max exponent) */
    idx_t deg_v = 1;          /* current degree of v (max exponent) */
    u[0] = (seq_t)1;
    v[0] = (seq_t)1;

    /* Build polynomials up to row r. */
    for (idx_t i = 2; i <= r; ++i) {
        /* New degrees after step i. */
        idx_t deg_u_new = (i > 2 ? i - 1 : 0);
        idx_t deg_v_new = i - 1;

        /* Compute u_new = u + v (with possible missing terms). */
        for (idx_t k = 0; k <= deg_u_new; ++k) {
            seq_t a = (k <= deg_u ? u[k] : 0);
            seq_t b = (k <= deg_v ? v[k] : 0);
            u_new[k] = a + b;
        }

        /* Initialize v_new to zero. */
        for (idx_t k = 0; k <= deg_v_new; ++k)
            v_new[k] = 0;

        /* x * u (shifted by one). */
        for (idx_t k = 0; k <= deg_u; ++k)
            v_new[k + 1] += u[k];

        /* (x+1) * v. */
        for (idx_t k = 0; k <= deg_v; ++k) {
            v_new[k]     += v[k];
            v_new[k + 1] += v[k];
        }

        /* Constant +1 term. */
        v_new[0] += (seq_t)1;

        /* Rotate buffers for next iteration. */
        seq_t *tmp = u; u = u_new; u_new = tmp; deg_u = deg_u_new;
        tmp = v; v = v_new; v_new = tmp; deg_v = deg_v_new;
    }

    /* Extract the desired coefficient from u. */
    seq_t result = u[col];

    /* Clean up. */
    free(u);
    free(v);
    free(u_new);
    free(v_new);

    return result;
}
