/*
 * A210597  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A210602; see the Formula section.
 * https://oeis.org/A210597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the coefficient of x^(c) in the polynomial u(r,x), where
 *            r = the row index such that Tri(r‑1) < n ≤ Tri(r) and c = n –
 *            Tri(r‑1) – 1. Recurrence: u(1,x) = 1, v(1,x) = 1 u(n,x) =
 *            2x*u(n‑1,x) + v(n‑1,x) + 1 v(n,x) = (x+1)*u(n‑1,x) + v(n‑1,x) +
 *            1 The k‑th coefficient (0 ≤ k < n) of u(n,x) is...
 * Range:     u8 n <= 24, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Determine the row r and column c (0‑based) for the flat index n. */
    idx_t r = 1;
    idx_t sum = 1;                    /* Tri(r) = r*(r+1)/2 */
    while (sum < n) {
        ++r;
        sum += r;
    }
    idx_t prev_sum = sum - r;         /* Tri(r‑1) */
    idx_t c = n - prev_sum - 1;       /* column in row r */

    /* Allocate arrays of length r for current and previous rows. */
    seq_t *u_prev = (seq_t *)calloc(r, sizeof(seq_t));
    seq_t *v_prev = (seq_t *)calloc(r, sizeof(seq_t));
    seq_t *u_curr = (seq_t *)calloc(r, sizeof(seq_t));
    seq_t *v_curr = (seq_t *)calloc(r, sizeof(seq_t));
    if (!u_prev || !v_prev || !u_curr || !v_curr) {
        free(u_prev); free(v_prev); free(u_curr); free(v_curr);
        return (seq_t)0;
    }

    /* Initial row (n = 1). */
    u_prev[0] = (seq_t)1;
    v_prev[0] = (seq_t)1;

    /* Build rows 2 … r. */
    for (idx_t row = 2; row <= r; ++row) {
        /* First coefficient (k = 0). */
        u_curr[0] = v_prev[0] + (seq_t)1;
        v_curr[0] = u_prev[0] + v_prev[0] + (seq_t)1;

        /* Middle coefficients: 1 ≤ k < row‑1. */
        for (idx_t k = 1; k < row - 1; ++k) {
            u_curr[k] = u_prev[k - 1] * (seq_t)2 + v_prev[k];
            v_curr[k] = u_prev[k] + u_prev[k - 1] + v_prev[k];
        }

        /* Last coefficient (k = row‑1). */
        u_curr[row - 1] = u_prev[row - 2] * (seq_t)2;
        v_curr[row - 1] = u_prev[row - 2];

        /* Swap current and previous rows for next iteration. */
        seq_t *tmp = u_prev; u_prev = u_curr; u_curr = tmp;
        tmp = v_prev;       v_prev = v_curr; v_curr = tmp;
    }

    /* The desired coefficient resides in u_prev[c]. */
    seq_t result = u_prev[c];

    /* Clean up. */
    free(u_prev);
    free(v_prev);
    free(u_curr);
    free(v_curr);

    return result;
}
