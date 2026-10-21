/*
 * A210550  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A210549; see the Formula section.
 * https://oeis.org/A210550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = coefficient of x^(k) in v(n,x), where the triangle of
 *            coefficients of v(n,x) is generated jointly with A210549 as
 *            described in the OEIS entry. a(1)=1, a(2)=2, a(3)=2, a(4)=2,
 *            a(5)=6, a(6)=4, …
 * Range:     u8 n <= 32, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */

    /* current row number (length of the arrays for this row) */
    idx_t row = 1;
    /* cumulative count of coefficients already produced */
    idx_t cumulative = 0;

    /* arrays holding coefficients of u and v for the current row */
    seq_t *u = (seq_t *)calloc(1, sizeof(seq_t));
    seq_t *v = (seq_t *)calloc(1, sizeof(seq_t));
    if (!u || !v) {                /* allocation failure – unlikely */
        free(u);
        free(v);
        return 0;
    }

    /* base polynomials for n = 1 */
    u[0] = (seq_t)1;
    v[0] = (seq_t)1;

    for (;;)
    {
        /* scan through coefficients of v(row-1,x) */
        for (idx_t k = 0; k < row; ++k)
        {
            if (cumulative + 1 == n)
            {
                seq_t ans = v[k];
                free(u);
                free(v);
                return ans;
            }
            ++cumulative;
        }

        /* prepare next row (n = row+1) */
        ++row;
        seq_t *u_next = (seq_t *)calloc(row, sizeof(seq_t));
        seq_t *v_next = (seq_t *)calloc(row, sizeof(seq_t));
        if (!u_next || !v_next)
        {
            free(u);
            free(v);
            free(u_next);
            free(v_next);
            return 0;
        }

        /* recurrence: u(n,x) = x*u(n-1,x)+x*v(n-1,x)+1
           v(n,x) = u(n-1,x)+2x*v(n-1,x)+1   */
        u_next[0] = (seq_t)1;
        v_next[0] = u[0] + (seq_t)1;

        for (idx_t k = 1; k < row; ++k)
        {
            /* u_next[k] = u_prev[k-1] + v_prev[k-1] */
            u_next[k] = u[k - 1] + v[k - 1];

            /* v_next[k] =
               (k < old_len ? u_prev[k] : 0) + 2 * v_prev[k-1]   */
            seq_t u_term = (k < (row - 1) ? u[k] : (seq_t)0);
            v_next[k] = u_term + (seq_t)2 * v[k - 1];
        }

        /* recycle memory for next iteration */
        free(u);
        free(v);
        u = u_next;
        v = v_next;
    }
}
