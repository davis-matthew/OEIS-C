/*
 * A210227  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A210228; see the Formula section.
 * https://oeis.org/A210227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = coefficient u(n, k) in the triangle of polynomials u(n,x)
 *            defined by u(1,x) = 1, u(n,x) = x*u(n‑1,x) + v(n‑1,x) + 1, where
 *            v(1,x) = 1 and v(n,x) = 2x*u(n‑1,x) + 2x*v(n‑1,x) + 1. The
 *            sequence a(n) lists the coefficients row by row
 * Range:     u8 n <= 25, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 ≥ n. */
    idx_t r = 1;
    while (((seq_t)r * ((seq_t)r + 1)) / 2 < n)
        ++r;

    /* Buffers for previous and current rows. */
    seq_t *u_prev = NULL;
    seq_t *v_prev = NULL;
    seq_t *u_curr = NULL;
    seq_t *v_curr = NULL;
    idx_t size_prev = 0, size_curr = 0;

    for (idx_t m = 1; m <= r; ++m)
    {
        size_curr = m;
        u_curr = (seq_t *)malloc(size_curr * sizeof(seq_t));
        v_curr = (seq_t *)malloc(size_curr * sizeof(seq_t));
        if (!u_curr || !v_curr)
        {
            free(u_prev); free(v_prev);
            free(u_curr); free(v_curr);
            return (seq_t)0;   /* allocation failure */
        }

        if (m == 1)        /* base row */
        {
            u_curr[0] = (seq_t)1;
            v_curr[0] = (seq_t)1;
        }
        else
        {
            /* k = 0 */
            u_curr[0] = v_prev[0] + (seq_t)1;
            v_curr[0] = (seq_t)1;

            /* k ≥ 1 */
            for (idx_t k = 1; k < size_curr; ++k)
            {
                /* u(n,x) coefficient */
                seq_t u_term = u_prev[k - 1];
                if (k < size_prev)       /* v(n‑1,x) has term for k */
                    u_term += v_prev[k];
                u_curr[k] = u_term;

                /* v(n,x) coefficient */
                seq_t t = u_prev[k - 1] + v_prev[k - 1];
                v_curr[k] = t + t;       /* multiply by 2 */
            }
        }

        /* Dispose of the previous row. */
        free(u_prev);
        free(v_prev);
        u_prev = u_curr;
        v_prev = v_curr;
        size_prev = size_curr;
        u_curr = NULL;
        v_curr = NULL;
    }

    /* Zero‑based offset of n within the last row. */
    seq_t prev_terms = ((seq_t)(r - 1) * (seq_t)r) / 2;
    seq_t offset_seq = (seq_t)n - prev_terms - 1;
    idx_t offset = (idx_t)offset_seq;

    seq_t result = u_prev[offset];

    free(u_prev);
    free(v_prev);
    return result;
}
