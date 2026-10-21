/*
 * A209583  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A209584; see the Formula section.
 * https://oeis.org/A209583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = coefficient of the polynomial u(r,x) at index k, where r
 *            = smallest integer such that r*(r+1)/2 >= n, and k = n -
 *            r*(r-1)/2 - 1 (0‑based into row r). The polynomials are defined
 *            by the recurrence u(n,x) = x*u(n-1,x) + v(n-1,x) v(n,x) =
 *            (x+1)*u(n-1,x) + (x+1)*v(n-1,x) + 1 u(1,x) = 1, v(1,x)...
 * Range:     u8 n <= 30, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n) {
    /* Space to hold previously computed rows */
    static seq_t **U = NULL;
    static seq_t **V = NULL;
    static size_t rows = 0;   /* number of rows already computed (max index) */

    /* Determine the row r containing the n-th element */
    idx_t r;
    {
        /* r = ceil((sqrt(8*n+1)-1)/2) */
        double d = sqrt((double)8 * (double)n + 1.0);
        r = (idx_t)((d - 1.0) / 2.0);
        if ((idx_t)((r * (r + 1)) / 2) < n) r++;
    }

    size_t rsize = (size_t)r;
    size_t k = (size_t)(n - (rsize * (rsize - 1) / 2) - 1);   /* index within row */

    /* Build rows up to r if needed */
    if (rsize > rows) {
        seq_t **U_new = (seq_t **)realloc(U, sizeof(seq_t *) * (rsize + 1));
        seq_t **V_new = (seq_t **)realloc(V, sizeof(seq_t *) * (rsize + 1));
        if (!U_new || !V_new) return 0;   /* allocation failure */
        U = U_new;
        V = V_new;

        for (size_t i = rows + 1; i <= rsize; ++i) {
            size_t len = i;   /* row i has i coefficients */
            seq_t *u_new = (seq_t *)malloc(sizeof(seq_t) * len);
            seq_t *v_new = (seq_t *)malloc(sizeof(seq_t) * len);
            if (!u_new || !v_new) return 0;   /* allocation failure */

            if (i == 1) {
                /* Initial row */
                for (size_t j = 0; j < len; ++j) {
                    u_new[i - 1] = (seq_t)1;   /* will be overwritten, we set below */
                }
                u_new[0] = (seq_t)1;
                v_new[0] = (seq_t)1;
            } else {
                seq_t *u_prev = U[i - 1];
                seq_t *v_prev = V[i - 1];
                size_t prev_len = i - 1;
                for (size_t j = 0; j < len; ++j) {
                    seq_t comp_u = (j > 0 ? u_prev[j - 1] : (seq_t)0);
                    seq_t comp_v = (j < prev_len ? v_prev[j] : (seq_t)0);
                    u_new[j] = comp_u + comp_v;

                    seq_t a = (j > 0 ? u_prev[j - 1] : (seq_t)0);
                    seq_t b = (j < prev_len ? u_prev[j] : (seq_t)0);
                    seq_t c = (j > 0 ? v_prev[j - 1] : (seq_t)0);
                    seq_t d = (j < prev_len ? v_prev[j] : (seq_t)0);
                    seq_t e = (j == 0 ? (seq_t)1 : (seq_t)0);
                    v_new[j] = a + b + c + d + e;
                }
            }

            U[i] = u_new;
            V[i] = v_new;
        }
        rows = rsize;
    }

    return U[rsize][k];
}
