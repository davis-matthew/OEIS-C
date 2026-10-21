/*
 * A210555  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A210556; see the Formula section.
 * https://oeis.org/A210555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = coefficient of x^(k-1) in u(r,x) where 
 *                 r = smallest integer with r(r+1)/2 >= n,
 *                 k = n - r(r-1)/2.  
 * The polynomials u and v are defined by  
 *                 u(1)=1, v(1)=1 and  
 *                 u(n)=x*u(n-1)+x*v(n-1)+1,  
 *                 v(n)=2x*u(n-1)+v(n-1)+1.  
 * The sequence lists the coefficients of u(n,x) row by row.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 >= n */
    idx_t r = 1;
    while (((__int128)r * (r + 1) / 2) < n)
        ++r;

    /* Offset within the row */
    idx_t prev = (idx_t)(((__int128)(r - 1) * r) / 2);
    idx_t k    = n - prev;          /* 1‑based index inside the row */

    /* Allocate arrays large enough for the largest needed row */
    seq_t *u_prev = (seq_t *)malloc(sizeof(seq_t) * (size_t)r);
    seq_t *u_curr = (seq_t *)malloc(sizeof(seq_t) * (size_t)r);
    seq_t *v_prev = (seq_t *)malloc(sizeof(seq_t) * (size_t)r);
    seq_t *v_curr = (seq_t *)malloc(sizeof(seq_t) * (size_t)r);
    if (!u_prev || !u_curr || !v_prev || !v_curr)
        return 0;  /* Allocation failure – undefined for the problem */

    /* Base case: u(1)=1, v(1)=1 */
    u_prev[0] = 1;
    v_prev[0] = 1;

    /* Build polynomials up to row r */
    for (idx_t row = 2; row <= r; ++row) {
        /* m = 0 */
        u_curr[0] = 1;                 /* constant term of u(row) */
        v_curr[0] = 1 + v_prev[0];     /* constant term of v(row) */

        /* m from 1 to row-1 */
        for (idx_t m = 1; m < row; ++m) {
            seq_t v_prev_m = (m < row - 1) ? v_prev[m] : (seq_t)0;
            u_curr[m] = u_prev[m - 1] + v_prev[m - 1];
            v_curr[m] = 2 * u_prev[m - 1] + v_prev_m;
        }

        /* Swap arrays for the next iteration */
        seq_t *tmp;
        tmp = u_prev; u_prev = u_curr; u_curr = tmp;
        tmp = v_prev; v_prev = v_curr; v_curr = tmp;
    }

    seq_t result = u_prev[k - 1];

    /* Free allocated memory */
    free(u_prev);
    free(u_curr);
    free(v_prev);
    free(v_curr);

    return result;
}
