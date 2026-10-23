/*
 * A179900  Triangle T(n,k) read by rows: coefficient of [x^k] of the
 *          polynomial p_n(x)=(5-x)*p_{n-1}(x)-p_{n-2}(x), p_0=1, p_1=5-x.
 * https://oeis.org/A179900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   * T(n,k) = 5*T(n‑1,k) - T(n‑1,k‑1) - T(n‑2,k) (with T(0,0)=1, *
 *            T(1,0)=5, T(1,1)=-1) * a(n) = T(r,k) where r is the unique row
 *            with n = r*(r+1)/2 + k * (row and column are zero‑based, rows
 *            are read left to right). * This function implements the above
 *            recurrence
 * Range:     i8 n <= 9, i16 n <= 27, i32 n <= 45, i64 n <= 45, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Determine the triangle row r and column k for the flatten index n */
    idx_t r = 0;
    for (;; ++r) {
        idx_t next_tri = (idx_t)(((__int128)(r + 1) * (r + 2)) / 2);
        if (next_tri > n) break;
    }
    idx_t offset = (idx_t)(((__int128)r * (r + 1)) / 2);
    idx_t k = n - offset;

    /* Old rows: prevprev = row i‑2, prev = row i‑1 */
    seq_t *prevprev = NULL;
    seq_t *prev = NULL;
    size_t len_prevprev = 0, len_prev = 0;

    /* Row 0: T(0,0)=1 */
    prevprev = (seq_t *)malloc(sizeof(seq_t));
    if (!prevprev) return (seq_t)0;
    prevprev[0] = (seq_t)1;
    len_prevprev = 1;
    if (r == 0) {
        seq_t res = prevprev[0];
        free(prevprev);
        return res;
    }

    /* Row 1: T(1,0)=5, T(1,1)=-1 */
    prev = (seq_t *)malloc(2 * sizeof(seq_t));
    if (!prev) { free(prevprev); return (seq_t)0; }
    prev[0] = (seq_t)5;
    prev[1] = (seq_t)-1;
    len_prev = 2;
    if (r == 1) {
        seq_t res = prev[k];
        free(prevprev);
        free(prev);
        return res;
    }

    /* Build rows 2 .. r */
    for (idx_t i = 2; i <= r; ++i) {
        size_t len_curr = i + 1;
        seq_t *curr = (seq_t *)malloc(len_curr * sizeof(seq_t));
        if (!curr) { free(prevprev); free(prev); return (seq_t)0; }

        for (size_t j = 0; j < len_curr; ++j) {
            seq_t t1 = (j < len_prev) ? (seq_t)5 * prev[j] : (seq_t)0;
            seq_t t2 = (j > 0 && (j - 1) < len_prev) ? prev[j - 1] : (seq_t)0;
            seq_t t3 = (j < len_prevprev) ? prevprev[j] : (seq_t)0;
            curr[j] = t1 - t2 - t3;
        }

        /* Shift rows for next iteration */
        free(prevprev);          /* free row i‑2 */
        prevprev = prev;         /* row i‑1 becomes row i‑2 */
        len_prevprev = len_prev;
        prev = curr;             /* current row becomes row i‑1 */
        len_prev = len_curr;
    }

    /* Result is the element k of the last row */
    seq_t result = prev[k];
    free(prevprev);
    free(prev);
    return result;
}
