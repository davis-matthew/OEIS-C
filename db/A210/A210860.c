/*
 * A210860  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A210861; see the Formula section of A210861.
 * https://oeis.org/A210860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the flattened list of coefficients of the polynomials *
 *            u(n,x) defined jointly with A210861: * u(1,x) = 1, v(1,x) = 1 *
 *            u(n,x) = u(n-1,x) + (x+1) * v(n-1,x) * v(n,x) = (x + n) *
 *            u(n-1,x) + x * v(n-1,x) * The sequence a(n) is obtained by
 *            concatenating, for each n ≥ 1, the * coefficients of...
 * Range:     u8 n <= 17, u16 n <= 47, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

seq_t a(idx_t n)
{
    /* n == 1 is handled specially */
    if (n <= 1) {
        return (seq_t)1;
    }

    /* start with row 1 */
    idx_t row = 1;
    seq_t *u_prev = alloc_seq(1);
    seq_t *v_prev = alloc_seq(1);
    u_prev[0] = (seq_t)1;
    v_prev[0] = (seq_t)1;
    idx_t tot = 1;                 /* elements already produced */

    while (1) {
        /* move to the next row */
        row++;
        size_t len = row;          /* number of coefficients in this row */
        seq_t *u_cur = alloc_seq(len);
        seq_t *v_cur = alloc_seq(len);
        seq_t n_seq = (seq_t)row;
        size_t prev_len = row - 1;

        /* compute the coefficients of u and v for this row */
        for (size_t j = 0; j < len; j++) {
            seq_t u_prev_j   = (j < prev_len)   ? u_prev[j]   : (seq_t)0;
            seq_t v_prev_j   = (j < prev_len)   ? v_prev[j]   : (seq_t)0;
            seq_t u_prev_jm1 = (j > 0 && j - 1 < prev_len) ? u_prev[j-1] : (seq_t)0;
            seq_t v_prev_jm1 = (j > 0 && j - 1 < prev_len) ? v_prev[j-1] : (seq_t)0;

            /* v(n,x) = (x+n) * u(n-1,x) + x * v(n-1,x) */
            v_cur[j] = n_seq * u_prev_j + u_prev_jm1 + v_prev_jm1;

            /* u(n,x) = u(n-1,x) + (x+1) * v(n-1,x) */
            u_cur[j] = u_prev_j + v_prev_j + (j > 0 ? v_prev_jm1 : (seq_t)0);
        }

        tot += len;
        if (tot >= n) {
            /* position of the requested term in this row (1‑based) */
            idx_t pos_in_row = n - (tot - len);
            idx_t idx0 = pos_in_row - 1;            /* 0‑based index */
            seq_t result = u_cur[idx0];

            /* free all allocated memory before returning */
            free_seq(u_prev);
            free_seq(v_prev);
            free_seq(u_cur);
            free_seq(v_cur);
            return result;
        }

        /* prepare for the next iteration */
        free_seq(u_prev);
        free_seq(v_prev);
        u_prev = u_cur;
        v_prev = v_cur;
    }
}
