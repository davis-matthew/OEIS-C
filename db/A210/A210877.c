/*
 * A210877  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A210876; see the Formula section.
 * https://oeis.org/A210877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the coefficient of x^k in the polynomial v(r,x), * where
 *            r is the least integer for which r(r+1)/2 ≥ n, * and k = n -
 *            r(r-1)/2 - 1 (0‑based index). * * u(1,x)=1, v(1,x)=1 *
 *            u(n,x)=x*u(n‑1,x)+v(n‑1,x)+1 * v(n,x)=x*u(n‑1,x)+x*v(n‑1,x)+x *
 *            The sequence a(n) lists all coefficients of v(n,x) r...
 * Range:     u8 n <= 51, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row r that contains the n-th coefficient. */
    idx_t r = 1;
    idx_t cum = 1;               /* cumulative number of terms up to row r   */
    while (cum < n) {
        ++r;
        cum += r;
    }

    /* Position within the row (1‑based). */
    idx_t prev_cum = cum - r;    /* cumulative terms up to row r‑1          */
    idx_t offset = n - prev_cum; /* 1‑based index inside the row           */
    idx_t idx = offset - 1;      /* 0‑based coefficient index             */

    /* The rows never exceed r = 200 for all reasonable n.
     * 200 is large enough for n up to 10 000 (row 141). */
    seq_t u[200]     = {0};
    seq_t v[200]     = {0};
    seq_t new_u[200] = {0};
    seq_t new_v[200] = {0};

    /* Base polynomial (n = 1). */
    u[0] = 1;
    v[0] = 1;
    idx_t cur_len = 1;         /* length of current row (number of coefficients) */

    /* Build rows up to r. */
    for (idx_t i = 2; i <= r; ++i) {
        /* coefficient j = 0 (constant term) */
        new_u[0] = v[0] + 1;  /* v(n‑1,0) + 1  */
        new_v[0] = 0;          /* constant term of v(n) is always 0 for n ≥ 2 */

        /* coefficients j ≥ 1 */
        for (idx_t j = 1; j < i; ++j) {
            /* new_u[j] = u(n‑1,j‑1) + v(n‑1,j)   */
            seq_t u_prev = u[j - 1];
            seq_t v_prev = (j < cur_len) ? v[j] : 0;
            new_u[j] = u_prev + v_prev;

            /* new_v[j] = u(n‑1,j‑1) + v(n‑1,j‑1) + (j==1 ? 1 : 0)  */
            seq_t u_prev2 = u[j - 1];
            seq_t v_prev2 = v[j - 1];
            seq_t add_x = (j == 1) ? 1 : 0;
            new_v[j] = u_prev2 + v_prev2 + add_x;
        }

        /* Copy new coefficients into the current arrays. */
        for (idx_t j = 0; j < i; ++j) {
            u[j] = new_u[j];
            v[j] = new_v[j];
        }
        cur_len = i;
    }

    /* Return the required coefficient of v(r,x). */
    return v[idx];
}
