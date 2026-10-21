/*
 * A209152  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A208339; see the Formula section.
 * https://oeis.org/A209152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A209152 – Triangle of coefficients of u(n,x)
 *  Recurrence:
 *    u(1,x) = 1
 *    v(1,x) = 1
 *    u(n,x) = u(n-1,x) + (x+1)*v(n-1,x)
 *    v(n,x) = x*u(n-1,x) + 2x*v(n-1,x) + 1
 *  The flattened sequence a(n) lists the coefficients of u(n,x)
 *  row by row (row 1: 1; row 2: 2,1; row 3: 3,5,3; …).  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1 – undefined for 0 */
        return 0;

    /* Handle the trivial first row without allocating memory */
    if (n == 1)
        return (seq_t)1;

    /* Find the smallest row r such that 1+2+...+r ≥ n */
    idx_t r = 0;
    while (((unsigned __int128)r * (r + 1)) / 2 < n)
        ++r;

    /* Allocate temporary arrays big enough for the largest row (size r)   */
    idx_t max_size = r;                     /* largest needed length */
    seq_t *u_prev = (seq_t *)malloc((max_size + 1) * sizeof(seq_t));
    seq_t *v_prev = (seq_t *)malloc((max_size + 1) * sizeof(seq_t));
    seq_t *u_cur  = (seq_t *)malloc((max_size + 1) * sizeof(seq_t));
    seq_t *v_cur  = (seq_t *)malloc((max_size + 1) * sizeof(seq_t));

    if (!u_prev || !v_prev || !u_cur || !v_cur) {
        /* Allocation failure – no value can be computed */
        free(u_prev); free(v_prev); free(u_cur); free(v_cur);
        return 0;
    }

    /* Initialise row 1 */
    u_prev[0] = (seq_t)1;
    v_prev[0] = (seq_t)1;

    idx_t cumulative = 1;   /* terms up through row 1 */

    /* Build rows 2 … r */
    for (idx_t cur_row = 2; cur_row <= r; ++cur_row) {

        /* Compute u_cur[0..cur_row-1] and v_cur[0..cur_row-1] */
        for (idx_t k = 0; k < cur_row; ++k) {
            seq_t u_prev_k   = (k < (cur_row - 1))   ? u_prev[k]   : (seq_t)0;
            seq_t v_prev_k   = (k < (cur_row - 1))   ? v_prev[k]   : (seq_t)0;
            seq_t v_prev_km1 = (k > 0)              ? v_prev[k-1] : (seq_t)0;

            u_cur[k] = u_prev_k + v_prev_k + v_prev_km1;

            if (k == 0) {
                v_cur[0] = (seq_t)1;
            } else {
                v_cur[k] = u_prev[k-1] + (seq_t)2 * v_prev[k-1];
            }
        }

        idx_t cumulative_prev = cumulative;
        cumulative += cur_row;

        /* Does n lie in this row? */
        if (n <= cumulative) {
            idx_t j = n - cumulative_prev - 1;   /* 0‑based index in row */
            seq_t result = u_cur[j];

            /* Clean up and return the answer */
            free(u_prev); free(v_prev); free(u_cur); free(v_cur);
            return result;
        }

        /* Prepare for next iteration: swap old and new rows. */
        seq_t *t = u_prev; u_prev = u_cur; u_cur = t;
        t = v_prev; v_prev = v_cur; v_cur = t;
    }

    /* Should never reach here – guard against bugs */
    free(u_prev); free(v_prev); free(u_cur); free(v_cur);
    return 0;
}
