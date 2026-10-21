/*
 * A209701  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A209702; see the Formula section.
 * https://oeis.org/A209701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the coefficient of x^(k) in the polynomial u(r,x) where
 *            u(1,x)=1, v(1,x)=1, and u(n,x)=x*u(n-1,x)+x*v(n-1,x),
 *            v(n,x)=2x*u(n-1,x)+(x+1)v(n-1,x)+1. Row r contributes the
 *            coefficients of u(r,x) for k=0..r-1 (constant term first). The
 *            sequence is read row by row
 * Range:     u8 n <= 33, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t **u_rows = NULL;          /* stored u coefficient rows */
static size_t u_rows_cap = 0;          /* current capacity of u_rows array */
static seq_t *prev_u = NULL;           /* pointer to previous row's u */
static seq_t *prev_v = NULL;           /* pointer to previous row's v */
static idx_t prev_len = 0;             /* length of previous row (row index) */

/* Ensure that rows up to r are computed and stored in u_rows. */
static void compute_rows_up_to(idx_t r)
{
    /* If no rows have been computed yet, create row 1. */
    if (u_rows_cap == 0) {
        size_t cap = 2;  /* allow indices 0 and 1 */
        u_rows = (seq_t **)malloc(sizeof(seq_t *) * cap);
        u_rows_cap = 1;  /* we fill up to index 1 now */
        u_rows[0] = NULL;       /* unused slot */

        /* Allocate row 1 for u and v */
        seq_t *row_u = (seq_t *)malloc(sizeof(seq_t) * 1);
        seq_t *row_v = (seq_t *)malloc(sizeof(seq_t) * 1);
        row_u[0] = 1;
        row_v[0] = 1;

        u_rows[1] = row_u;
        prev_u = row_u;
        prev_v = row_v;
        prev_len = 1;
    }

    /* Compute additional rows up to r */
    while (u_rows_cap < (size_t)r) {
        size_t i = u_rows_cap + 1;               /* new row index */
        size_t i_len = i;                        /* length of this row */

        seq_t *u_cur = (seq_t *)malloc(sizeof(seq_t) * i_len);
        seq_t *v_cur = (seq_t *)malloc(sizeof(seq_t) * i_len);

        if (i_len == 1) {
            /* Should not happen after the first row */
            u_cur[0] = 1;
            v_cur[0] = 1;
        } else {
            /* u_i[0] = 0 for i > 1 */
            u_cur[0] = 0;
            for (size_t k = 1; k < i_len; ++k) {
                /* u(i,k) = prev_u(k-1) + prev_v(k-1) */
                u_cur[k] = prev_u[k - 1] + prev_v[k - 1];
            }

            /* v_i[0] = prev_v[0] + 1 */
            v_cur[0] = prev_v[0] + 1;
            for (size_t k = 1; k < i_len; ++k) {
                /* v(i,k) = 2*prev_u(k-1) + prev_v(k-1) + prev_v(k) */
                seq_t term1 = prev_u[k - 1] + prev_u[k - 1]; /* 2*prev_u(k-1) */
                seq_t term2 = (k - 1 < prev_len) ? prev_v[k - 1] : 0;
                seq_t term3 = (k < prev_len)     ? prev_v[k] : 0;
                v_cur[k] = term1 + term2 + term3;
            }
        }

        /* Store the new row and update the previous pointers for next iteration */
        u_rows_cap = i;
        size_t new_cap = u_rows_cap + 1;
        u_rows = (seq_t **)realloc(u_rows, sizeof(seq_t *) * new_cap);
        u_rows[i] = u_cur;

        prev_u = u_cur;
        prev_v = v_cur;
        prev_len = i;
    }
}

/* Return the nth term of OEIS A209701 */
seq_t a(idx_t n)
{
    if (n == 0) { /* offset is 1, but guard against zero */
        return 0;
    }

    /* Find the row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    while (((idx_t)r * (r + 1)) / 2 < n) {
        ++r;
    }

    /* Ensure the required rows are computed */
    compute_rows_up_to(r);

    /* Compute position within the row */
    idx_t total_before = ((idx_t)(r - 1) * r) / 2;   /* sum of lengths of rows 1..r-1 */
    idx_t pos = n - total_before;                   /* 1 .. r */

    if (pos == 1) {          /* constant term */
        return (r == 1) ? (seq_t)1 : (seq_t)0;
    } else {                 /* coefficient of x^(pos-1) in u(r,x) */
        return u_rows[r][pos - 1];
    }
}
