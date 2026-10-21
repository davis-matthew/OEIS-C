/*
 * A208909  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A208930; see the Formula section.
 * https://oeis.org/A208909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle of coefficients of polynomials u(n,x) defined by
   u(1,x)=1, v(1,x)=1 and recurrence:
   u(n,x)=u(n-1,x)+2x*v(n-1,x),
   v(n,x)=(x+1)*u(n-1,x)+2x*v(n-1,x)+1.
   The a(n) sequence is the row‑wise flattening of the coefficients of u(n,x). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Determine the row m that contains the n‑th element. */
    idx_t m = 0;
    idx_t tri = 0;
    while (tri < n) {
        ++m;
        tri += m;
    }

    /* Position of n within row m (1‑based). */
    idx_t prev_tri = tri - m;
    idx_t pos = n - prev_tri;          /* 1 ≤ pos ≤ m */

    seq_t *u_prev = NULL;
    seq_t *v_prev = NULL;
    size_t len_prev = 0;

    for (size_t row = 1; row <= (size_t)m; ++row) {
        size_t len = row;
        seq_t *u_cur = (seq_t*)malloc(len * sizeof(seq_t));
        seq_t *v_cur = (seq_t*)malloc(len * sizeof(seq_t));
        if (!u_cur || !v_cur)
            return (seq_t)0;          /* allocation failure */

        if (row == 1) {
            u_cur[0] = 1;
            v_cur[0] = 1;
        } else {
            /* compute u(row,x) */
            for (size_t k = 0; k < len; ++k) {
                seq_t up  = (k < len_prev) ? u_prev[k] : 0;
                seq_t vp  = (k > 0)       ? v_prev[k-1] : 0;
                u_cur[k] = up + (seq_t)2 * vp;
            }
            /* compute v(row,x) */
            for (size_t k = 0; k < len; ++k) {
                seq_t up      = (k < len_prev) ? u_prev[k] : 0;
                seq_t up_prev = (k > 0)       ? u_prev[k-1] : 0;
                seq_t vp_shift = (k > 0)     ? v_prev[k-1] : 0;
                v_cur[k] = up + up_prev + (seq_t)2 * vp_shift;
                if (k == 0) v_cur[0] += 1;
            }
        }

        /* discard previous row */
        free(u_prev);
        free(v_prev);
        u_prev = u_cur;
        v_prev = v_cur;
        len_prev = len;
    }

    seq_t result = u_prev[pos - 1];

    free(u_prev);
    free(v_prev);
    return result;
}
