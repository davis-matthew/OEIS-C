/*
 * A210795  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A210796; see the Formula section.
 * https://oeis.org/A210795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   grow_rows
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A210795 – triangle of coefficients of polynomials u(n,x)
 *
 * Formula (recurrence):
 *    u(1,x) = 1
 *    v(1,x) = 1
 *
 *    u(n,x) = u(n-1,x) + x*v(n-1,x) + 1
 *    v(n,x) = (x+2)*u(n-1,x) + (x-1)*v(n-1,x)
 *
 * The sequence a(n) is obtained by reading the rows of u(n,x)
 * (coefficients of constant term first) left‑to‑right, row by row.
 * The first few rows are:
 *    1
 *    2 1
 *    3 2 2
 *    4 5 5 3
 *    ...
 *
 * Initial values (a(1) = 1, a(2) = 2, a(3) = 1, a(4) = 3, …).
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_rows.h"

/* The sequence entry a(idx_t n). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined, but safe return */

    /* static state for memoisation */
    static bool initialized = false;
    static seq_t **rows = NULL;      /* array of pointers to rows (u) */
    static size_t rows_cap = 0;
    static idx_t current_rows = 0;      /* number of rows currently stored */

    static seq_t *u_prev = NULL;        /* previous row (u) */
    static seq_t *v_prev = NULL;        /* previous row (v) */
    static idx_t prev_len = 0;          /* length of previous row == current_rows */

    /* Initialise on first call */
    if (!initialized) {
        initialized = true;
        grow_rows(&rows_cap, &rows);

        /* Row 1 */
        seq_t *row1 = (seq_t *)malloc(sizeof(seq_t));
        if (!row1) abort();
        row1[0] = (seq_t)1;
        rows[0] = row1;
        current_rows = 1;
        prev_len = 1;
        u_prev = row1;

        v_prev = (seq_t *)malloc(sizeof(seq_t));
        if (!v_prev) abort();
        v_prev[0] = (seq_t)1;
    }

    /* Find the smallest r such that T_r = r*(r+1)/2 >= n */
    idx_t r_needed = current_rows;
    while ((r_needed * (r_needed + 1)) / 2 < n) ++r_needed;

    /* Extend precomputation up to r_needed */
    for (idx_t r = current_rows + 1; r <= r_needed; ++r) {
        /* Allocate new rows of length r */
        seq_t *u_new = (seq_t *)malloc(r * sizeof(seq_t));
        seq_t *v_new = (seq_t *)malloc(r * sizeof(seq_t));
        if (!u_new || !v_new) abort();

        /* Compute coefficients of the new row */
        for (idx_t k = 0; k < r; ++k) {
            /* u_new[k] = u_prev[k] + v_prev[k-1] + (k==0?1:0) */
            seq_t upk   = (k < prev_len) ? u_prev[k] : (seq_t)0;
            seq_t vpkm1 = (k > 0)       ? v_prev[k - 1] : (seq_t)0;
            seq_t const1 = (k == 0) ? (seq_t)1 : (seq_t)0;
            u_new[k] = upk + vpkm1 + const1;

            /* v_new[k] = u_prev[k-1] + 2*u_prev[k] + v_prev[k-1] - v_prev[k] */
            seq_t upkm1 = (k > 0)       ? u_prev[k - 1] : (seq_t)0;
            seq_t upk_2 = (k < prev_len) ? (seq_t)2 * u_prev[k] : (seq_t)0;
            seq_t vpkm1v = (k > 0)       ? v_prev[k - 1] : (seq_t)0;
            seq_t vpk = (k < prev_len)   ? v_prev[k] : (seq_t)0;
            v_new[k] = upkm1 + upk_2 + vpkm1v - vpk;
        }

        /* Store the new row */
        if (current_rows >= rows_cap) grow_rows(&rows_cap, &rows);
        rows[current_rows] = u_new;

        /* Free old v row and update pointers */
        free(v_prev);
        v_prev = v_new;

        /* Update state for next iteration */
        u_prev = u_new;
        prev_len = r;
        ++current_rows;
    }

    /* Locate the row and position of the desired index n */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) ++r;
    idx_t prev_sum = ((r - 1) * r) / 2;
    idx_t k = n - prev_sum - 1;   /* zero‑based column within the row */

    return rows[r - 1][k];
}
