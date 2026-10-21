/*
 * A209752  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A209751; see the Formula section.
 * https://oeis.org/A209752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   u(n,x)=x*u(n-1,x)+(x+1)*v(n-1,x), v(n,x)=u(n-1,x)+2x*v(n-1,x)+1,
 *            u(1,x)=1, v(1,x)=1. a(n) is the n‑th coefficient of v(n,x) read
 *            row‑major (row 1 has 1 term, row 2 has 2 terms, …)
 * Range:     u8 n <= 25, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static state: current row size and the coefficient arrays of u and v */
    static idx_t cur = 0;          /* number of coefficients in stored row (row count) */
    static seq_t *u = NULL;
    static seq_t *v = NULL;

    /* find the row that contains index n by solving r*(r+1)/2 >= n */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    /* initialise the first row if not yet done */
    if (cur == 0) {
        cur = 1;
        u = (seq_t *)malloc(sizeof(seq_t));
        v = (seq_t *)malloc(sizeof(seq_t));
        u[0] = (seq_t)1;
        v[0] = (seq_t)1;
    }

    /* extend stored row to reach row r */
    while (cur < r) {
        idx_t new_size = cur + 1;
        seq_t *newU = (seq_t *)malloc(sizeof(seq_t) * new_size);
        seq_t *newV = (seq_t *)malloc(sizeof(seq_t) * new_size);

        /* compute newU and newV from the previous u and v */
        for (idx_t i = 0; i < new_size; ++i) {
            seq_t u_prev_i     = (i < cur) ? u[i]     : (seq_t)0;
            seq_t v_prev_i     = (i < cur) ? v[i]     : (seq_t)0;
            seq_t u_prev_i_m1  = (i > 0)  ? u[i-1] : (seq_t)0;
            seq_t v_prev_i_m1  = (i > 0)  ? v[i-1] : (seq_t)0;

            /* u(n,x) = x*u(n-1,x) + (x+1)*v(n-1,x) */
            newU[i] = u_prev_i_m1               /* from x*u_prev */
                    + v_prev_i_m1               /* from x*v_prev */
                    + v_prev_i;                /* from +v_prev */

            /* v(n,x) = u(n-1,x) + 2x*v(n-1,x) + 1 */
            if (i == 0) {
                newV[i] = u_prev_i + (seq_t)1;
            } else {
                newV[i] = u_prev_i + 2 * v_prev_i_m1;
            }
        }

        free(u);
        free(v);
        u = newU;
        v = newV;
        cur = new_size;
    }

    /* locate coefficient within row r */
    idx_t prev_sum = (r * (r - 1)) / 2;   /* number of terms before row r */
    idx_t j = n - prev_sum - 1;           /* 0‑based column index */
    return v[(size_t)j];
}
