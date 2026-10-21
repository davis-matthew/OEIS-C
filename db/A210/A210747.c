/*
 * A210747  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A210748; see the Formula section.
 * https://oeis.org/A210747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the coefficient of x^(c-1) in the polynomial u(r,x),
 *            where u(1,x)=1, v(1,x)=1, and for n>1:
 *            u(n,x)=2x*u(n-1,x)+(x+1)*v(n-1,x)+1,
 *            v(n,x)=(x+1)*u(n-1,x)+(x+1)*v(n-1,x)+1. The triangle of the
 *            coefficients of u is flattened row by row. For a given n, let r
 *            be the unique integer with r(r-1)/2...
 * Range:     u8 n <= 17, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* offset is 1; undefined for n<1 */

    /* determine row r such that r(r-1)/2 < n ≤ r(r+1)/2 */
    idx_t r = 1;
    while ((unsigned __int128)r * (r + 1) / 2 < n) ++r;

    idx_t prev_sum = r * (r - 1) / 2;     /* sum of elements in rows < r */
    idx_t col = n - prev_sum;            /* 1 … r */
    /* col-1 is the degree of the desired coefficient (0‑based) */

    /* initialise u(1,x) and v(1,x) */
    seq_t *U = alloc_seq_array(1);
    seq_t *V = alloc_seq_array(1);
    U[0] = (seq_t)1;
    V[0] = (seq_t)1;

    for (idx_t cur = 2; cur <= r; ++cur)
    {
        size_t cur_sz = (size_t)cur;
        seq_t *U_new = alloc_seq_array(cur_sz);
        seq_t *V_new = alloc_seq_array(cur_sz);
        idx_t prev_sz = cur - 1;

        for (idx_t d = 0; d < cur; ++d)
        {
            /* helpers for previous row, treating out‑of‑range as 0 */
            seq_t u_prev_dminus = (d == 0) ? (seq_t)0 : U[d - 1];
            seq_t v_prev_dminus = (d == 0) ? (seq_t)0 : V[d - 1];
            seq_t u_prev_d = (d < prev_sz) ? U[d] : (seq_t)0;
            seq_t v_prev_d = (d < prev_sz) ? V[d] : (seq_t)0;

            /* compute coefficient for u */
            seq_t u_cur = 2 * u_prev_dminus + v_prev_d
                          + (d == 0 ? (seq_t)0 : v_prev_dminus);
            if (d == 0) u_cur += (seq_t)1;
            U_new[d] = u_cur;

            /* compute coefficient for v */
            seq_t v_cur = u_prev_d + v_prev_d
                          + u_prev_dminus + v_prev_dminus;
            if (d == 0) v_cur += (seq_t)1;
            V_new[d] = v_cur;
        }

        free_seq_array(U);
        free_seq_array(V);
        U = U_new;
        V = V_new;
    }

    seq_t result = U[col - 1];
    free_seq_array(U);
    free_seq_array(V);
    return result;
}
