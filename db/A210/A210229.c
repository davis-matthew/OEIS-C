/*
 * A210229  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A210230; see the Formula section.
 * https://oeis.org/A210229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A210229: triangle of coefficients of u(n,x).
   Rows are read from left to right, top to bottom. */

#include <stdlib.h>
#include "../utils/alloc_seq_array.h"

static seq_t **u_tbl = NULL;
static seq_t **v_tbl = NULL;
static idx_t max_row = 0;

/* Ensure that rows 1 … r are computed and stored. */
static void ensure_row(idx_t r)
{
    if (max_row == 0)
    {
        u_tbl = (seq_t **)malloc(2 * sizeof(seq_t *));
        v_tbl = (seq_t **)malloc(2 * sizeof(seq_t *));
        u_tbl[0] = NULL;          /* unused index 0 */
        v_tbl[0] = NULL;
        u_tbl[1] = alloc_seq_array(1);
        v_tbl[1] = alloc_seq_array(1);
        u_tbl[1][0] = (seq_t)1;
        v_tbl[1][0] = (seq_t)1;
        max_row   = 1;
    }

    while (max_row < r)
    {
        idx_t   new_len = max_row + 1;          /* length of the new row   */
        seq_t *prev_u  = u_tbl[max_row];
        seq_t *prev_v  = v_tbl[max_row];
        idx_t  prev_len = max_row;

        seq_t *u_cur = alloc_seq_array(new_len);
        seq_t *v_cur = alloc_seq_array(new_len);

        /* k == 0 */
        u_cur[0] = prev_v[0] + (seq_t)1;
        v_cur[0] = prev_u[0] + prev_v[0] + (seq_t)1;

        /* k >= 1 */
        for (idx_t k = 1; k < new_len; ++k)
        {
            seq_t bu = prev_u[k - 1];
            seq_t bv = (k < prev_len) ? prev_v[k] : (seq_t)0;
            u_cur[k] = bu + bv;

            seq_t pu  = prev_u[k - 1];
            seq_t pu2 = (k < prev_len) ? prev_u[k] : (seq_t)0;
            seq_t pv  = (k < prev_len) ? prev_v[k] : (seq_t)0;
            v_cur[k] = pu + pu2 + pv;
        }

        u_tbl = (seq_t **)realloc(u_tbl, (new_len + 1) * sizeof(seq_t *));
        v_tbl = (seq_t **)realloc(v_tbl, (new_len + 1) * sizeof(seq_t *));
        u_tbl[new_len] = u_cur;
        v_tbl[new_len] = v_cur;

        ++max_row;
    }
}

/* Return the n-th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined element */

    /* Find row r such that r*(r+1)/2 ≥ n. */
    idx_t r = 1;
    idx_t tri = 1;
    while (tri < n)
    {
        ++r;
        tri = r * (r + 1) / 2;
    }

    ensure_row(r);

    idx_t prev_total = (r - 1) * r / 2;
    idx_t offset = n - prev_total - 1;    /* zero‑based column within the row */

    return u_tbl[r][offset];
}
