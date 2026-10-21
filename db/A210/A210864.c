/*
 * A210864  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A210865; see the Formula section.
 * https://oeis.org/A210864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th element of the flattened triangle of *
 *            coefficients of the polynomials u(n,x) defined by * u(n,x) =
 *            u(n-1,x) + x·v(n-1,x) + 1, * v(n,x) = (x + n)·u(n-1,x) +
 *            x·v(n-1,x), * with u(1,x)=1 and v(1,x)=1. * The triangle has n
 *            coefficients per row; the sequence is formed by * concatenat...
 * Range:     u8 n <= 24, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   alloc_seq_t, free_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_seq_t.h"
#include "../utils/free_seq_t.h"

#ifdef __cplusplus
    
    
#else
    
    
#endif

seq_t a(idx_t n)
{
    /* n is 1‑based. For n<1, return 0 (undefined behaviour for the
       OEIS offset). */
    if (n < 1U) return (seq_t)0;

    /* Find the row R that contains the n‑th term.
       R satisfies R*(R+1)/2 >= n. */
    idx_t row = 0;
    idx_t cum = 0;
    while (cum < n) {
        ++row;
        cum += row;
    }
    /* cum now equals the total number of terms up to row.
       Index (0‑based) of the desired coefficient within the row. */
    idx_t idx_in_row = n - (cum - row) - 1;

    /* Initial polynomial (n=1). */
    seq_t *u_prev = alloc_seq_t(1);
    seq_t *v_prev = alloc_seq_t(1);
    u_prev[0] = (seq_t)1;
    v_prev[0] = (seq_t)1;
    size_t prev_len = 1;

    if (row == 1) {
        seq_t result = u_prev[0]; /* only coefficient when row==1 */
        free_seq_t(u_prev);
        free_seq_t(v_prev);
        return result;
    }

    /* Iterate rows 2..row. */
    for (idx_t k = 2; k <= row; ++k) {
        size_t cur_len = prev_len + 1;
        seq_t *u_cur = alloc_seq_t(cur_len);
        seq_t *v_cur = alloc_seq_t(cur_len);

        /* i = 0 */
        u_cur[0] = u_prev[0] + (seq_t)1;
        v_cur[0] = (seq_t)k * u_prev[0];

        /* i >= 1 */
        for (size_t i = 1; i < cur_len; ++i) {
            seq_t u_prev_i   = (i < prev_len) ? u_prev[i] : (seq_t)0;
            seq_t u_prev_im1 = u_prev[i - 1];          /* always valid */

            seq_t v_prev_i   = (i < prev_len) ? v_prev[i] : (seq_t)0;
            seq_t v_prev_im1 = v_prev[i - 1];          /* always valid */

            u_cur[i] = u_prev_i + v_prev_im1;

            v_cur[i] = (seq_t)k * u_prev_i + u_prev_im1 + v_prev_im1;
        }

        /* Free previous arrays and move to next iteration. */
        free_seq_t(u_prev);
        free_seq_t(v_prev);
        u_prev = u_cur;
        v_prev = v_cur;
        prev_len = cur_len;
    }

    /* Extract the desired coefficient from the final row. */
    seq_t result = u_prev[idx_in_row];

    /* Clean up. */
    free_seq_t(u_prev);
    free_seq_t(v_prev);

    return result;
}
