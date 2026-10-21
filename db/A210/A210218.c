/*
 * A210218  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A210217; see the Formula section.
 * https://oeis.org/A210218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A210218:  Triangle of coefficients of polynomials v(n,x)
 *
 *  Recurrence (x is a formal indeterminate):
 *      u(1,x) = 1,     v(1,x) = 1
 *      u(n,x) = x*u(n‑1,x) + v(n‑1,x) + 1
 *      v(n,x) = x*u(n‑1,x) + 2x*v(n‑1,x) + 1
 *
 *  The sequence a is obtained by listing the coefficients of all v(n,x)
 *  row by row; the nth term a(n) is the coefficient that appears at index
 *  n in that triangular enumeration.  The implementation recomputes
 *  only the required row, running in O(√n) time and O(√n) memory.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n=0, but harmless */

    /* Determine the row k that contains the n‑th element. */
    idx_t k = 1;
    while (k * (k + 1) / 2 < n)
        ++k;

    /* Number of elements in all previous rows. */
    idx_t prev_rows = k * (k - 1) / 2;
    /* Column (1‑based) within row k. */
    idx_t j = n - prev_rows;

    /* Allocate the first row. */
    seq_t *u_prev = (seq_t *)malloc(sizeof(seq_t));
    seq_t *v_prev = (seq_t *)malloc(sizeof(seq_t));
    if (!u_prev || !v_prev) return (seq_t)0;  /* allocation failure */

    u_prev[0] = 1;
    v_prev[0] = 1;

    /* Build rows 2 .. k. */
    for (idx_t cur = 2; cur <= k; ++cur) {
        size_t cur_size  = (size_t)cur;
        size_t prev_size = (size_t)(cur - 1);

        seq_t *u_cur = (seq_t *)malloc(cur_size * sizeof(seq_t));
        seq_t *v_cur = (seq_t *)malloc(cur_size * sizeof(seq_t));
        if (!u_cur || !v_cur) {
            free(u_prev); free(v_prev);
            return (seq_t)0;   /* allocation failure */
        }

        for (size_t idx = 0; idx < cur_size; ++idx) {
            /* u(cur,idx) = (idx>0?u_prev[idx-1]:0) + (idx<prev?v_prev[idx]:0)
             *               + (idx==0?1:0)
             */
            seq_t term_u_from_prev = (idx > 0) ? u_prev[idx - 1] : (seq_t)0;
            seq_t term_v_from_prev = (idx < prev_size) ? v_prev[idx] : (seq_t)0;
            u_cur[idx] = term_u_from_prev + term_v_from_prev
                         + ((idx == 0) ? 1 : 0);

            /* v(cur,idx) = (idx>0?u_prev[idx-1]:0) + 2*(idx>0?v_prev[idx-1]:0)
             *               + (idx==0?1:0)
             */
            seq_t term_u_shift = (idx > 0) ? u_prev[idx - 1] : (seq_t)0;
            seq_t term_v_shift = (idx > 0) ? v_prev[idx - 1] : (seq_t)0;
            v_cur[idx] = term_u_shift + 2 * term_v_shift
                         + ((idx == 0) ? 1 : 0);
        }

        free(u_prev);
        free(v_prev);
        u_prev = u_cur;
        v_prev = v_cur;
    }

    /* Extract the requested coefficient. */
    seq_t result = v_prev[(size_t)(j - 1)];

    free(u_prev);
    free(v_prev);
    return result;
}
