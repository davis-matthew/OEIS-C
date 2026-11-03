/*
 * A210199  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A210200; see the Formula section.
 * https://oeis.org/A210199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = coefficient of x^(pos-1) in u(row) where u(1,x)=1,
 *            v(1,x)=1, u(n,x)=u(n-1,x)+v(n-1,x)+1,
 *            v(n,x)=(x+1)u(n-1,x)+x*v(n-1,x)+1. This generates the triangle
 *            of coefficients of u(n,x)
 * Range:     u8 n <= 30, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/*------------------------------------------------------------*/
static inline void add_row(idx_t target_row,
                          seq_t **out_u, idx_t *len_u,
                          seq_t **out_v, idx_t *len_v)
{
    for (idx_t r = 2; r <= target_row; ++r) {
        idx_t old_u_len = *len_u;   /* length of u(r-1) */
        idx_t old_v_len = *len_v;   /* length of v(r-1) */

        idx_t new_u_len = r - 1;    /* length of u(r)   */
        idx_t new_v_len = r;        /* length of v(r)   */

        /* Allocate new rows (zero-initialised) */
        seq_t *u_new = mem_new(seq_t, (size_t)new_u_len);
        seq_t *v_new = mem_new(seq_t, (size_t)new_v_len);

        /* ---- compute v(r) ------------------------------------- */
        /* (x+1)*u(r-1) contributes u to v_new[i] and u to v_new[i+1] */
        for (idx_t i = 0; i < old_u_len; ++i) {
            v_new[i]     += (*out_u)[i];
            v_new[i + 1] += (*out_u)[i];
        }
        /* x * v(r-1) shifts v by one position to the right */
        for (idx_t i = 0; i < old_v_len; ++i) {
            v_new[i + 1] += (*out_v)[i];
        }
        /* constant term +1 */
        v_new[0] += 1;

        /* ---- compute u(r) ------------------------------------- */
        /* constant term */
        u_new[0] = (*out_u)[0] + (*out_v)[0] + 1;
        /* higher coefficients: u[i] + v[i] (treat missing as 0) */
        for (idx_t i = 1; i < new_u_len; ++i) {
            seq_t u_val = (i < old_u_len) ? (*out_u)[i] : 0;
            u_new[i] = u_val + (*out_v)[i];
        }

        /* free old rows */
        mem_free(*out_u);
        mem_free(*out_v);

        /* update pointers and lengths for next iteration */
        *out_u = u_new;  *len_u = new_u_len;
        *out_v = v_new;  *len_v = new_v_len;
    }
}

/*------------------------------------------------------------*/
static inline idx_t pos_in_row(idx_t n, idx_t row)
{
    /* Number of terms before row 'row' */
    idx_t before = (row == 1) ? 0 : 1 + (row - 1) * (row - 2) / 2;
    return n - before;     /* 1‑based position in the row */
}

/*------------------------------------------------------------*/
static inline idx_t row_from_index(idx_t n)
{
    /* Find smallest r such that 1 + r(r-1)/2 >= n  (C(r) >= n) */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        idx_t cum = 1 + mid * (mid - 1) / 2;   /* safe for 64‑bit */
        if (cum >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined, but harmless */

    /* Determine which row and position within that row. */
    idx_t row = row_from_index(n);
    idx_t pos  = pos_in_row(n, row);   /* 1‑based */

    /* Start with row 1: u(1,x)=1, v(1,x)=1 */
    seq_t *u_cur = mem_new(seq_t, 1);
    seq_t *v_cur = mem_new(seq_t, 1);
    *u_cur = 1;
    *v_cur = 1;
    idx_t len_u = 1, len_v = 1;

    if (row > 1) {
        add_row(row, &u_cur, &len_u, &v_cur, &len_v);
    }

    /* Extract the required coefficient.  For row 1 pos will always be 1. */
    seq_t result = u_cur[pos - 1];

    /* Clean up */
    mem_free(u_cur);
    mem_free(v_cur);

    return result;
}
