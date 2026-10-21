/*
 * A209770  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A209769; see the Formula section.
 * https://oeis.org/A209770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth coefficient in the triangle of coefficients of
 *            the polynomial v(n,x) defined by u(1,x)=1, v(1,x)=1 and
 *            u(n,x)=x*u(n-1,x)+(x+1)*v(n-1,x),
 *            v(n,x)=(x+1)*u(n-1,x)+v(n-1,x)+1. The triangle is read
 *            row‑by‑row with row n containing n coefficients
 *            a_{n,0},…,a_{n,n-1}. This function comput...
 * Range:     u8 n <= 29, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   nth_to_row_col
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/nth_to_row_col.h"

/* --------- static helper ---------- */
static idx_t current_max_row = 0;   /* highest row that has been generated    */
static seq_t *a_store = NULL;      /* flattened array of all a_{r,k} values   */
static size_t store_len = 0;        /* number of elements actually stored     */
static size_t store_cap = 0;        /* current capacity of a_store             */

static seq_t *a_prev = NULL;       /* coefficients of v for previous row     */
static seq_t *b_prev = NULL;       /* coefficients of u for previous row     */
static idx_t prev_len = 0;          /* length of previous row (current_max_row) */

static void ensure_store_cap(size_t needed) {
    if (store_len + needed > store_cap) {
        size_t new_cap = store_cap ? store_cap * 2 : 64;
        while (store_len + needed > new_cap) new_cap *= 2;
        a_store = (seq_t*)realloc(a_store, new_cap * sizeof(seq_t));
        store_cap = new_cap;
    }
}

/* Generate rows up to r, filling a_store and updating a_prev/b_prev. */
static void generate_rows(idx_t r) {
    if (current_max_row == 0) {
        /* initialise row 1 */
        idx_t len = 1;
        a_prev = (seq_t*)malloc(sizeof(seq_t) * len);
        b_prev = (seq_t*)malloc(sizeof(seq_t) * len);
        a_prev[0] = (seq_t)1;
        b_prev[0] = (seq_t)1;
        ensure_store_cap(len);
        a_store[store_len++] = a_prev[0];
        current_max_row = 1;
        prev_len = len;
    }
    for (idx_t ncur = current_max_row + 1; ncur <= r; ++ncur) {
        idx_t len = ncur;
        seq_t *a_curr = (seq_t*)malloc(sizeof(seq_t) * len);
        seq_t *b_curr = (seq_t*)malloc(sizeof(seq_t) * len);
        for (idx_t k = 0; k < len; ++k) {
            /* compute b_curr[k] = b_{prev}[k-1] + a_{prev}[k-1] + a_{prev}[k]  */
            seq_t term1 = (k >= 1) ? b_prev[k-1] : (seq_t)0;
            seq_t term2 = (k >= 1) ? a_prev[k-1] : (seq_t)0;
            seq_t term3 = (k < prev_len) ? a_prev[k] : (seq_t)0;
            b_curr[k] = term1 + term2 + term3;

            /* compute a_curr[k] = b_{prev}[k-1] + b_{prev}[k] + a_{prev}[k] + (k==0?1:0) */
            seq_t t1 = (k >= 1) ? b_prev[k-1] : (seq_t)0;
            seq_t t2 = (k < prev_len) ? b_prev[k] : (seq_t)0;
            seq_t t3 = (k < prev_len) ? a_prev[k] : (seq_t)0;
            seq_t val = t1 + t2 + t3 + ((k == 0) ? (seq_t)1 : (seq_t)0);
            a_curr[k] = val;
        }
        /* store the row */
        ensure_store_cap(len);
        for (idx_t k = 0; k < len; ++k)
            a_store[store_len++] = a_curr[k];

        /* free previous row arrays and move to new ones */
        free(a_prev);
        free(b_prev);
        a_prev = a_curr;
        b_prev = b_curr;
        prev_len = len;
        current_max_row = ncur;
    }
}

seq_t a(idx_t n) {
    /* Determine row and column of the requested term. */
    idx_t row, col;
    nth_to_row_col(n, &row, &col);
    /* Generate rows up to that row if necessary. */
    if (current_max_row < row) {
        generate_rows(row);
    }
    /* Compute index in flattened storage. */
    idx_t offset = (row - 1) * row / 2;
    idx_t idx = offset + col;
    return a_store[idx];
}
