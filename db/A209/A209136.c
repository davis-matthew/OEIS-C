/*
 * A209136  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A209135; see the Formula section.
 * https://oeis.org/A209136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* -------------  Formula ------------- */
/* A209136 is the triangle of coefficients of the polynomials v(n,x) */
/* generated jointly with A209135 by:                                 */
/*     u(n,x) = u(n-1,x) + (x+1)*v(n-1,x),                            */
/*     v(n,x) = 2x*u(n-1,x) + (x+1)*v(n-1,x),                         */
/* with u(1,x)=1, v(1,x)=1.  a(n) is the n‑th entry read along the
   antidiagonals of the triangle (row 1:1; row 2:1,3; row 3:1,8,5; ...). */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

static seq_t **rows = NULL;          /* rows[1] -> v(1,.) etc.          */
static seq_t *u_prev = NULL;         /* coefficients from u(n-1,.)      */
static seq_t *v_prev = NULL;         /* coefficients from v(n-1,.)      */
static idx_t   max_row = 0;          /* last row that has been built    */
static size_t rows_cap = 0;          /* allocated capacity for rows     */

/* Ensure that rows[1..needed] exist.                                      */
static void
ensure_rows(idx_t needed)
{
    if (needed <= max_row)
        return;

    /* Allocate space for the rows array if necessary.                     */
    if (rows_cap == 0) {
        rows_cap = 2;                /* accommodate indices 0 and 1      */
        rows = (seq_t**)malloc(rows_cap * sizeof(seq_t*));
        if (!rows) abort();
        rows[0] = NULL;              /* unused                            */
        rows[1] = NULL;
    } else if ((size_t)needed >= rows_cap) {
        size_t new_cap = rows_cap * 2;
        if ((size_t)needed + 1 > new_cap)
            new_cap = (size_t)needed + 1;
        seq_t **tmp = (seq_t**)realloc(rows, new_cap * sizeof(seq_t*));
        if (!tmp) abort();
        rows = tmp;
        rows_cap = new_cap;
    }

    /* ----- Initial known row ------------------------------------------ */
    if (max_row == 0) {
        /* allocate the first row (len = 1)                               */
        u_prev = seq_alloc_new_array(1);
        v_prev = seq_alloc_new_array(1);
        u_prev[0] = (seq_t)1;
        v_prev[0] = (seq_t)1;

        rows[1] = seq_alloc_new_array(1);
        rows[1][0] = (seq_t)1;
        max_row = 1;
    }

    /* ----- Grow rows until the required one --------------------------- */
    while (max_row < needed) {
        idx_t n = max_row + 1;            /* new row index                  */
        size_t len = (size_t)n;           /* number of coefficients          */

        seq_t *u_curr = seq_alloc_new_array(len);
        seq_t *v_curr = seq_alloc_new_array(len);

        for (idx_t k = 0; k < len; ++k) {
            seq_t sum_u = (seq_t)0;
            seq_t sum_v = (seq_t)0;

            if (k < n-1) { /* terms from the previous row           */
                sum_u += u_prev[k];
                sum_u += v_prev[k];
                sum_v += v_prev[k];
            }
            if (k > 0) {    /* shifted terms (x factor)             */
                sum_u += v_prev[k-1];
                sum_v += (seq_t)2 * u_prev[k-1] + v_prev[k-1];
            }
            u_curr[k] = sum_u;
            v_curr[k] = sum_v;
        }

        /* store new row and prepare for next iteration                 */
        rows[(size_t)n] = v_curr;

        /* free old arrays, keep new ones for the next round             */
        seq_free_array(u_prev);
        seq_free_array(v_prev);
        u_prev = u_curr;
        v_prev = v_curr;

        max_row = n;
    }
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;              /* undefined for n<1 */

    /* Find the row r such that r(r+1)/2 >= n.                           */
    idx_t r = (idx_t)1;
    idx_t tri = (idx_t)1;          /* r(r+1)/2 for current r          */
    while (tri < n) {
        ++r;
        tri += r;                  /* advance to next triangular number */
    }
    idx_t prev_tri = tri - r;      /* sum of 1..(r-1)                  */
    idx_t k = n - prev_tri - 1;    /* zero‑based column index           */

    ensure_rows(r);
    return rows[(size_t)r][(size_t)k];
}
