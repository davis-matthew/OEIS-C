/*
 * A208526  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A208527; see the Formula section.
 * https://oeis.org/A208526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A208526 – Triangle of coefficients of polynomials u(n,x) jointly
   generated with A208527.

   Recurrence (u for the polynomial, v auxiliary):
     u(1,x) = 1
     v(1,x) = 1
     u(n,x) = u(n-1,x) + x * v(n-1,x)
     v(n,x) = 2x*u(n-1,x) + 2x*v(n-1,x) + 1

   a(n) is the flattened sequence of coefficients of u(r,x) where
   r is the unique integer such that r*(r-1)/2 < n <= r*(r+1)/2.
   The index in that row is
        k = n - r*(r-1)/2 - 1   (0‑based)
   Finally a(n) = coefficient of x^k in u(r,x).
   The coefficients are computed in `seq_t`.
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for offset 1, defensive */
    
    /* find row r and position k */
    idx_t r = 1;
    idx_t cum = 1;               /* cumulative terms up to current row */
    while (cum < n) {
        ++r;
        cum += r;
    }
    idx_t prev = (r - 1) * r / 2;
    idx_t k = n - prev - 1;      /* 0‑based index within row r */

    /* iterative construction of u and v rows */
    idx_t prev_len = 0;
    seq_t *u_prev = NULL;
    seq_t *v_prev = NULL;

    /* row 1 */
    prev_len = 1;
    u_prev = (seq_t*)malloc(sizeof(seq_t) * prev_len);
    v_prev = (seq_t*)malloc(sizeof(seq_t) * prev_len);
    u_prev[0] = 1;
    v_prev[0] = 1;

    if (r == 1) {
        seq_t result = u_prev[0];
        free(u_prev);
        free(v_prev);
        return result;
    }

    /* rows 2 .. r */
    for (idx_t row = 2; row <= r; ++row) {
        idx_t cur_len = row;
        seq_t *u_curr = (seq_t*)malloc(sizeof(seq_t) * cur_len);
        seq_t *v_curr = (seq_t*)malloc(sizeof(seq_t) * cur_len);

        u_curr[0] = 1;
        v_curr[0] = 1;
        for (idx_t i = 1; i < cur_len; ++i) {
            seq_t u_contrib = (i < prev_len) ? u_prev[i] : 0;
            u_curr[i] = u_contrib + v_prev[i - 1];
            v_curr[i] = 2 * (u_prev[i - 1] + v_prev[i - 1]);
        }

        free(u_prev);
        free(v_prev);
        u_prev = u_curr;
        v_prev = v_curr;
        prev_len = cur_len;
    }

    /* result is coefficient at position k in last computed u_prev */
    seq_t result = u_prev[k];

    free(u_prev);
    free(v_prev);
    return result;
}
