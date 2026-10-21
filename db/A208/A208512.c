/*
 * A208512  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A208511; see the Formula section.
 * https://oeis.org/A208512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = coefficient of x^(k) in v(r,x), where r is the row number
 *            such that r(r-1)/2 < n ≤ r(r+1)/2 and k = n - r(r-1)/2 - 1.
 *            Recurrence: u(1,x)=1, v(1,x)=1; u(n,x)=u(n-1,x)+x*v(n-1,x);
 *            v(n,x)=u(n-1,x)+2x*v(n-1,x)+1. a(n) is obtained from the
 *            triangular array of coefficients of v(n,x)
 * Range:     u8 n <= 32, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* n is at least 1 according to the OEIS offset */
    /* Find the row number r such that 1+2+...+r >= n */
    idx_t r = 1;
    idx_t sum = 1;
    while (sum < n) {
        ++r;
        sum += r;
    }
    /* cumulative sum up to previous row (r-1) */
    idx_t prev_sum = sum - r;
    /* index within row (0‑based) */
    idx_t k = n - prev_sum - 1;

    /* Allocate arrays for polynomials up to degree r-1 */
    seq_t *u   = alloc_seq_array(r);
    seq_t *v   = alloc_seq_array(r);
    seq_t *u_n = alloc_seq_array(r);
    seq_t *v_n = alloc_seq_array(r);
    if (!u || !v || !u_n || !v_n) return 0;   /* allocation failed */

    /* Base case: row 1 */
    u[0] = 1;
    v[0] = 1;

    /* Build polynomials up to row r */
    for (idx_t cur = 2; cur <= r; ++cur) {
        /* u_cur[0] = u_prev[0] */
        u_n[0] = u[0];
        /* middle terms: u_cur[i] = u_prev[i] + v_prev[i-1]  for i=1..cur-2 */
        for (idx_t i = 1; i < cur-1; ++i) {
            u_n[i] = u[i] + v[i-1];
        }
        /* highest term: u_cur[cur-1] = v_prev[cur-2] */
        u_n[cur-1] = v[cur-2];

        /* v_cur[0] = u_prev[0] + 1 */
        v_n[0] = u[0] + (seq_t)1;
        /* middle terms: v_cur[i] = u_prev[i] + 2*v_prev[i-1] */
        for (idx_t i = 1; i < cur-1; ++i) {
            v_n[i] = u[i] + (v[i-1] << 1);
        }
        /* highest term: v_cur[cur-1] = 2*v_prev[cur-2] */
        v_n[cur-1] = v[cur-2] << 1;

        /* Swap pointers for next iteration */
        seq_t *tmp;
        tmp = u; u = u_n; u_n = tmp;
        tmp = v; v = v_n; v_n = tmp;
    }

    /* Result is the k-th coefficient of v (row r) */
    seq_t result = v[k];

    /* Free allocated memory */
    free_seq_array(u);
    free_seq_array(v);
    free_seq_array(u_n);
    free_seq_array(v_n);

    return result;
}
