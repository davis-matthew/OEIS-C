/*
 * A103718  Triangle of coefficients of certain polynomials used with prime
 *          numbers as variables in the computation of the array A103728.
 * https://oeis.org/A103718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(n) is the n-th element of the triangle a(k,m) where k is
 *            chosen so that the first index of row k is start = k*(k+1)/2,
 *            then m = n – start. The triangle satisfies a(0,0) = 1 a(k,0) =
 *            (k–1)! + k·a(k‑1,0) (k≥1) a(k,m) = –a(k‑1,m‑1) + k·a(k‑1,m)
 *            (k≥0, 0<m≤k)
 * Range:     i8 n <= 14, i16 n <= 35, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* locate the row k containing the index n */
    idx_t k = 0;
    idx_t start = 0;
    while (n >= start + k + 1) {
        start += k + 1;
        k++;
    }

    seq_t *prev = NULL;   /* previous row */
    idx_t prev_len = 0;
    seq_t fact_minus1 = 1;   /* (k‑1)! for current k (initially irrelevant) */
    seq_t ans = 0;

    for (idx_t cur_k = 0; cur_k <= k; ++cur_k) {
        idx_t len = cur_k + 1;
        seq_t *cur = (seq_t*)malloc(len * sizeof(*cur));

        if (cur_k == 0) {
            cur[0] = 1;  /* a(0,0) */
        } else {
            cur[0] = fact_minus1 + (cur_k) * prev[0];   /* a(k,0) */
        }

        for (idx_t m = 1; m < len; ++m) {
            seq_t prev_m_minus_1 = (m - 1 < prev_len) ? prev[m - 1] : 0;
            seq_t prev_m = (m < prev_len) ? prev[m] : 0;
            cur[m] = -prev_m_minus_1 + (cur_k) * prev_m;  /* a(k,m) */
        }

        if (cur_k == k) {
            idx_t local_index = n - start;
            ans = cur[local_index];
        }

        /* free previous row and advance */
        if (prev) free(prev);
        prev = cur;
        prev_len = len;

        /* update factorial for the next iteration */
        if (cur_k > 0) {
            fact_minus1 = fact_minus1 * cur_k;   /* becomes cur_k! */
        }
    }

    /* free the final row */
    if (prev) free(prev);
    return ans;
}
