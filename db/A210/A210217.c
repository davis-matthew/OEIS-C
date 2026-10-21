/*
 * A210217  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A210218; see the Formula section.
 * https://oeis.org/A210217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (OEIS A210217):
 *  u(1,x) = 1,            v(1,x) = 1
 *  u(n,x) = x*u(n‑1,x) + v(n‑1,x) + 1
 *  v(n,x) = x*u(n‑1,x) + 2x*v(n‑1,x) + 1
 *  a(n) lists the coefficients of u(n,x) (constant term first)
 *  in row‑major order for n = 1,2,3,…  (no simple closed form). */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Find the row r (1‑based) such that r(r+1)/2 ≥ n. */
    idx_t r = 0;
    while (((seq_t)r * (r + 1)) / 2 < n)
        ++r;

    /* Index of the term inside row r (0‑based). */
    idx_t pos = n - (idx_t)(((seq_t)(r - 1) * r) / 2) - 1;

    /* Allocate arrays for u and v coefficients of the current row. */
    seq_t *u = (seq_t*)malloc(sizeof(seq_t));
    seq_t *v = (seq_t*)malloc(sizeof(seq_t));
    u[0] = 1;
    v[0] = 1;

    /* Generate rows 2 … r (inclusive). */
    for (idx_t i = 2; i <= r; ++i) {
        seq_t *new_u = (seq_t*)calloc(i, sizeof(seq_t));
        seq_t *new_v = (seq_t*)calloc(i, sizeof(seq_t));

        /* Constant terms. */
        new_u[0] = v[0] + 1;
        new_v[0] = 1;

        /* Other coefficients. */
        for (idx_t k = 1; k < i; ++k) {
            seq_t term_u = u[k - 1];
            seq_t term_v = (k < i - 1) ? v[k] : 0;
            new_u[k] = term_u + term_v;

            seq_t term_v_cur = u[k - 1] + 2 * v[k - 1];
            new_v[k] = term_v_cur;
        }

        free(u);
        free(v);
        u = new_u;
        v = new_v;
    }

    seq_t result = u[pos];
    free(u);
    free(v);
    return result;
}
