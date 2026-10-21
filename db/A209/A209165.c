/*
 * A209165  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A209164; see the Formula section.
 * https://oeis.org/A209165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   use the recurrence u(1,x)=1, v(1,x)=1
 *            u(n,x)=u(n-1,x)+(x+1)*v(n-1,x) v(n,x)=(x+1)*u(n-1,x)+v(n-1,x)+1
 *            The sequence a(n) is the coefficients of v(n,x) read from the
 *            constant term upward, flattened by rows. a(1) = 1 (v(1,x)=1)
 *            a(2) = 3, a(3) = 1 (v(2,x)=x+3) a(4) = 6, a(5) = 4, a(6) = 1
 *            (v(3,x)=x^2...
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* find row r such that T_{r-1} < n <= T_r, where T_k = k(k+1)/2 */
    idx_t r = 0;
    idx_t t = 0;               /* cumulative count up to current row r */
    while (t < n) {
        r++;
        t += r;
    }
    idx_t prev_t = t - r;      /* T_{r-1} */
    idx_t c = n - prev_t - 1;  /* zero‑based position inside the row */

    /* triangular arrays of coefficients, maximum possible row <= 200 */
    seq_t u[200] = {0};
    seq_t v[200] = {0};
    u[0] = 1;
    v[0] = 1;

    for (idx_t i = 2; i <= r; ++i) {
        seq_t new_u[200] = {0};
        seq_t new_v[200] = {0};

        for (idx_t j = 0; j < i; ++j) {
            seq_t u_prev = u[j];                     /* u(n-1,x) coeff */
            seq_t v_prev = v[j];                     /* v(n-1,x) coeff */
            seq_t shift_v = (j > 0) ? v[j-1] : 0;    /* (x+1)*v shift left */
            seq_t shift_u = (j > 0) ? u[j-1] : 0;    /* (x+1)*u shift left */

            /* new_u[j] = u_prev + v_prev + shift_v */
            new_u[j] = u_prev + v_prev + shift_v;

            /* new_v[j] = u_prev + shift_u + v_prev + (j==0?1:0) */
            seq_t tmp = u_prev + v_prev + shift_u;
            if (j == 0) tmp += 1;
            new_v[j] = tmp;
        }

        /* copy new arrays back */
        for (idx_t j = 0; j < i; ++j) {
            u[j] = new_u[j];
            v[j] = new_v[j];
        }
    }

    return v[c];
}
