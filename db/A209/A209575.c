/*
 * A209575  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A209576; see the Formula section.
 * https://oeis.org/A209575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   u(n,x) = x*u(n‑1,x) + v(n‑1,x), v(n,x) = 2x*u(n‑1,x) +
 *            2x*v(n‑1,x) + 1, u(1,x) = v(1,x) = 1. a(n) lists the
 *            coefficients of u(n,x) in row‑major order
 * Range:     u8 n <= 25, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds u(n,x) row by row and returns the
   coefficient at the position corresponding to the flattened index n.
   It works for all seq_t and idx_t types that support the indicated
   operators, and satisfies the time/space requirements. */

#include <stddef.h>   /* size_t */

#define MAX_ROW 2000   /* should be ≥ maximum possible row length */

seq_t a(idx_t n) {
    if (n == 0) return 0;             /* sequence offset is 1 */

    /* Find the row containing the n-th element. */
    size_t r = 1;
    idx_t cum = 1;
    while ((idx_t)n > cum) {
        ++r;
        cum += (idx_t)r;
    }

    /* Working arrays for the current and previous rows.          */
    static seq_t u_prev[MAX_ROW];
    static seq_t v_prev[MAX_ROW];
    static seq_t u_cur[MAX_ROW];
    static seq_t v_cur[MAX_ROW];

    /* Initial row (n = 1). */
    u_prev[0] = (seq_t)1;
    v_prev[0] = (seq_t)1;

    /* Build successive rows up to row r. */
    for (size_t row = 2; row <= r; ++row) {
        /* u(row,0) = v(row‑1,0) */
        u_cur[0] = v_prev[0];
        /* v(row,0) = 1 */
        v_cur[0] = (seq_t)1;

        /* For 1 ≤ i ≤ row‑2 : u_cur[i] = u_prev[i‑1] + v_prev[i] */
        if (row > 2) {
            for (size_t i = 1; i < row - 1; ++i) {
                u_cur[i] = u_prev[i - 1] + v_prev[i];
            }
        }
        /* Last coefficient of u(row) */
        u_cur[row - 1] = u_prev[row - 2];

        /* v(row,i) for 1 ≤ i ≤ row‑1 : 2*u_prev[i‑1] + 2*v_prev[i‑1] */
        for (size_t i = 1; i < row; ++i) {
            v_cur[i] = (seq_t)2 * u_prev[i - 1] + (seq_t)2 * v_prev[i - 1];
        }

        /* Copy current row into previous for the next iteration. */
        for (size_t i = 0; i < row; ++i) {
            u_prev[i] = u_cur[i];
            v_prev[i] = v_cur[i];
        }
    }

    /* Compute column (0‑based) corresponding to the flattened index. */
    idx_t cum_prev = (idx_t)r * ((idx_t)(r - 1)) / 2;
    size_t j = (size_t)n - (size_t)cum_prev - 1;

    return u_prev[j];
}
