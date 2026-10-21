/*
 * A209153  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A208340; see the Formula section.
 * https://oeis.org/A209153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = coefficient of u(n,x) where u(1,x)=1, v(1,x)=1;
 *            u(n,x)=u(n-1,x)+(x+1)*v(n-1,x);
 *            v(n,x)=x*u(n-1,x)+(x+1)*v(n-1,x)+1; a(n) lists the coefficients
 *            of u(n,x) row‑wise
 * Range:     u8 n <= 24, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* static storage for previously computed rows */
    static seq_t **u_rows = NULL;
    static seq_t *v_row = NULL;
    static idx_t max_row = 0;
    static size_t rows_cap = 0;

    if (max_row == 0) {
        /* allocate for at least row 1 */
        if (rows_cap < 2) {
            u_rows = (seq_t**)malloc(sizeof(seq_t*) * 2);
            rows_cap = 2;
        }
        seq_t *row1 = (seq_t*)malloc(sizeof(seq_t) * 1);
        row1[0] = (seq_t)1;
        u_rows[1] = row1;

        seq_t *v1 = (seq_t*)malloc(sizeof(seq_t) * 1);
        v1[0] = (seq_t)1;
        v_row = v1;
        max_row = 1;
    }

    /* determine the row r containing position n */
    idx_t r = (idx_t)((sqrtl((long double)8 * n + 1) - 1) / 2);
    while ((r * (r + 1)) / 2 < n) ++r;

    /* build rows up to r */
    for (idx_t cur = max_row + 1; cur <= r; ++cur) {
        idx_t prev_len = cur - 1;       /* length of previous rows */
        idx_t new_len = cur;            /* length of current row */

        /* ensure storage for the new row */
        if (cur >= rows_cap) {
            size_t new_cap = rows_cap ? rows_cap * 2 : 2;
            while (new_cap <= (size_t)cur) new_cap *= 2;
            u_rows = (seq_t**)realloc(u_rows, new_cap * sizeof(seq_t*));
            rows_cap = new_cap;
        }

        seq_t *u_prev = u_rows[cur - 1];
        seq_t *new_u = (seq_t*)malloc(sizeof(seq_t) * new_len);
        seq_t *new_v = (seq_t*)malloc(sizeof(seq_t) * new_len);

        for (idx_t k = 0; k < new_len; ++k) {
            /* terms from v_prev for (x+1)*v_prev */
            seq_t v_k = 0, v_km1 = 0;
            if (k < prev_len) v_k = v_row[k];
            if (k > 0 && k - 1 < prev_len) v_km1 = v_row[k - 1];
            seq_t term_v = v_k + v_km1;

            /* u_new[k] = u_prev[k] + term_v */
            seq_t u_k = 0;
            if (k < prev_len) u_k = u_prev[k];
            new_u[k] = u_k + term_v;

            /* v_new[k] = ((k>0?u_prev[k-1]:0) + term_v + (k==0?1:0)) */
            seq_t u_km1 = 0;
            if (k > 0 && k - 1 < prev_len) u_km1 = u_prev[k - 1];
            seq_t const_one = (k == 0 ? (seq_t)1 : (seq_t)0);
            new_v[k] = u_km1 + term_v + const_one;
        }

        /* free old v_row and store new row */
        free(v_row);
        v_row = new_v;
        u_rows[cur] = new_u;
        max_row = cur;
    }

    /* compute offset within the row */
    idx_t prev_term_count = (r - 1) * r / 2;
    idx_t offset = n - prev_term_count - 1;
    return u_rows[r][offset];
}
