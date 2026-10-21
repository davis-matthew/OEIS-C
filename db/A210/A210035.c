/*
 * A210035  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A210036; see the Formula section.
 * https://oeis.org/A210035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the coefficient of x^k in the polynomial u(r,x)
 * where r and k are determined by the row‑major enumeration of the
 * triangle of coefficients of u(n,x).  The polynomials satisfy
 *      u(n,x) = u(n-1,x) + v(n-1,x) + 1,
 *      v(n,x) = u(n-1,x) + 2x*v(n-1,x) + 1
 * with u(1,x)=1, v(1,x)=1.  
 * Row r (r ≥ 1) contains r−1 coefficients of u(r,x) (row 1 has a single
 * coefficient).  The sequence a(n) is obtained by reading these
 * coefficients row‑wise, starting with a(1)=1.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* not defined, but safe */

    /* Find the row r (≥1) that contains position n.  The number of
       coefficients up to row r is T(r) = (r‑1)r/2 + 1. */
    idx_t r = 1;
    idx_t prev_T = 0;          /* T(r‑1) */
    idx_t T = 1;               /* T(r) for current r */
    while (T < n) {
        r++;
        prev_T = T;
        T = (r - 1) * r / 2 + 1;
    }

    /* Column index j (0‑based) within row r. */
    size_t j = (size_t)(n - (prev_T + 1));

    /* Allocate buffers for two consecutive rows.  The maximum needed
       length is r (row 1 has length 1, row r has r−1 coefficients). */
    size_t max_len = (size_t)r;
    seq_t *u_prev = (seq_t*)calloc(max_len, sizeof(seq_t));
    seq_t *v_prev = (seq_t*)calloc(max_len, sizeof(seq_t));
    seq_t *u_curr = (seq_t*)calloc(max_len, sizeof(seq_t));
    seq_t *v_curr = (seq_t*)calloc(max_len, sizeof(seq_t));

    if (!u_prev || !v_prev || !u_curr || !v_curr) {
        /* Allocation failed; return 0 as a safe default. */
        free(u_prev); free(v_prev); free(u_curr); free(v_curr);
        return (seq_t)0;
    }

    /* Initial polynomials for n = 1. */
    u_prev[0] = (seq_t)1;
    v_prev[0] = (seq_t)1;

    /* Build successive rows up to r. */
    for (idx_t cur = 2; cur <= r; ++cur) {
        size_t cur_size = (size_t)cur;
        size_t prev_size = cur_size - 1;      /* = cur-1 */

        for (size_t idx = 0; idx < cur_size; ++idx) {
            if (idx == 0) {
                seq_t u0 = prev_size > 0 ? u_prev[0] : (seq_t)0;
                seq_t v0 = prev_size > 0 ? v_prev[0] : (seq_t)0;
                u_curr[0] = u0 + v0 + (seq_t)1;
                v_curr[0] = u0 + (seq_t)1;
            } else {
                seq_t u_prev_j = idx < prev_size ? u_prev[idx] : (seq_t)0;
                seq_t v_prev_j = idx < prev_size ? v_prev[idx] : (seq_t)0;
                u_curr[idx] = u_prev_j + v_prev_j;

                seq_t v_prev_j_1 = v_prev[idx - 1];
                v_curr[idx] = u_prev_j + (seq_t)2 * v_prev_j_1;
            }
        }

        /* Swap previous and current buffers for next iteration. */
        seq_t *tmp = u_prev;  u_prev = u_curr;  u_curr = tmp;
        tmp = v_prev;         v_prev = v_curr;  v_curr = tmp;
    }

    seq_t result = u_prev[j];

    /* Free allocated memory. */
    free(u_prev); free(v_prev); free(u_curr); free(v_curr);
    return result;
}
