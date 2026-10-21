/*
 * A209172  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A209413; see the Formula section.
 * https://oeis.org/A209172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = coefficient u(n,x)[k] of the polynomial u(n,x) defined by
 *            * u(1,x) = 1, v(1,x) = 1 * u(n,x) = x*u(n-1,x) + v(n-1,x) *
 *            v(n,x) = u(n-1,x) + 2*x*v(n-1,x) * a(n) is the (n-1)-st element
 *            when the triangle of u(n,x) coefficients is * flattened row by
 *            row
 * Range:     u8 n <= 40, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define MAX_ROW 2000   /* sufficient for n up to at least 10^6 */

static seq_t u_prev[MAX_ROW];
static seq_t v_prev[MAX_ROW];
static seq_t u_curr[MAX_ROW];
static seq_t v_curr[MAX_ROW];

seq_t a(idx_t n) {
    /* Find the row r and column k that contain the n-th element
     * (rows are 1-indexed, each of length r). */
    idx_t r = 0;
    idx_t sum = 0;
    while (sum < n) {
        ++r;
        sum += r;
    }
    idx_t sum_prev = sum - r;          /* cumulative terms before row r */
    idx_t k0 = n - sum_prev - 1;       /* 0‑based column in row r */

    /* Initialize row 1 (u[1]=[1], v[1]=[1]) */
    u_prev[0] = 1;
    v_prev[0] = 1;

    if (r == 1) {
        /* Only the first element exists */
        return u_prev[0];
    }

    /* Compute rows 2 ... r iteratively using the recurrence. */
    for (idx_t cur = 2; cur <= r; ++cur) {
        /* u_curr[0] = v_prev[0] ;
         * v_curr[0] = u_prev[0] ;
         * For k >= 1:
         *   u_curr[k] = u_prev[k-1] + v_prev[k]   (v_prev[k] = 0 if k>=cur-1)
         *   v_curr[k] = u_prev[k] + 2*v_prev[k-1] (u_prev[k] = 0 if k>=cur-1)
         */
        u_curr[0] = v_prev[0];
        v_curr[0] = u_prev[0];

        for (idx_t k = 1; k < cur; ++k) {
            seq_t a = u_prev[k-1];
            seq_t b = (k < cur-1) ? v_prev[k] : 0;
            u_curr[k] = a + b;

            seq_t c = (k < cur-1) ? u_prev[k] : 0;
            seq_t d = v_prev[k-1];
            v_curr[k] = c + (d + d);   /* 2*d = d + d */
        }

        /* Prepare for next iteration: the current row becomes the previous row. */
        for (idx_t k = 0; k < cur; ++k) {
            u_prev[k] = u_curr[k];
            v_prev[k] = v_curr[k];
        }
    }

    return u_curr[k0];
}
