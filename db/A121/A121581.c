/*
 * A121581  Triangle read by rows: T(n,k) is the number of deco polyominoes of
 *          height n having k cells in the second column (n>=1, k>=0). A deco
 *          polyomino is a directed column-convex polyomino in which the
 *          height, measured along the diagonal, is attained only in the last
 *          column.
 * https://oeis.org/A121581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   row r of T(n,k): P_r(s) = coefficient of s^k in the triangle of
 *            deco polyominoes. Row sums are factorials. P_1(s)=1. Recurrence
 *            derived from the OEIS generating‑polynomial definition: P_r(s) =
 *            P_{r-1}(s) + (r-1)·R_{r-1}(s) R_r(s) = s·R_{r-1}(s) +
 *            (r-1)!·∑_{i=1}^{r-1} s^i where R_{r-1}(s)=Q(r-1,s,...
 * Range:     u8 n <= 22, u16 n <= 46, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   a_compute_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a_compute_row.h"

seq_t a(idx_t n) {
    /* Find row r and column k from linear index n (1‑based). */
    idx_t r = 1;
    /* Use a simple loop; n <= around 10^5 for performance requirement. */
    while ((r * (r + 1) / 2) < n) ++r;
    idx_t sum_prev = (r - 1) * r / 2;
    idx_t k = n - sum_prev - 1;           /* 0‑based column index */

    /* Compute row r */
    size_t P_len = (size_t)r;
    seq_t *P = (seq_t *)malloc(sizeof(seq_t) * P_len);
    /* Build rows up to r */
    size_t P_cur_len = 1;
    seq_t *P_cur = (seq_t *)malloc(sizeof(seq_t));
    P_cur[0] = (seq_t)1;                           /* P_1(s)=1 */

    size_t R_len = 2;
    seq_t *R_cur = (seq_t *)malloc(sizeof(seq_t) * 2);
    R_cur[0] = (seq_t)0;
    R_cur[1] = (seq_t)1;

    seq_t fact = (seq_t)1;                          /* (current row)! */

    for (idx_t i = 2; i <= r; ++i) {
        size_t new_P_len = (size_t)i;
        seq_t *new_P = (seq_t *)malloc(sizeof(seq_t) * new_P_len);
        for (size_t j = 0; j < new_P_len; ++j) new_P[j] = 0;

        /* add old P */
        for (size_t j = 0; j < P_cur_len; ++j) new_P[j] += P_cur[j];
        /* add (i-1)*old R */
        seq_t mult = (seq_t)(i - 1);
        for (size_t j = 0; j < R_len; ++j) new_P[j] += mult * R_cur[j];

        /* New R */
        size_t new_R_len = (size_t)(i + 1);
        seq_t *new_R = (seq_t *)malloc(sizeof(seq_t) * new_R_len);
        for (size_t j = 0; j < new_R_len; ++j) new_R[j] = 0;
        /* shift old R */
        for (size_t j = 0; j < R_len; ++j) new_R[j + 1] += R_cur[j];
        /* add (i-1)! * ∑_{m=1}^{i-1} s^m */
        for (size_t m = 1; m <= (size_t)(i - 1); ++m) new_R[m] += fact;

        /* free old arrays */
        free(P_cur); free(R_cur);

        /* update for next iteration */
        P_cur = new_P; P_cur_len = new_P_len;
        R_cur = new_R; R_len = new_R_len;
        fact *= (seq_t)i;                        /* fact becomes i! */
    }

    seq_t result = P_cur[k];
    free(P_cur); free(R_cur);
    return result;
}
