/*
 * A209572  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A209571; see the Formula section.
 * https://oeis.org/A209572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the coefficient of x^{p} in v(r,x), where r is the
 *            unique integer such that r(r‑1)/2 < n ≤ r(r+1)/2 and p = n –
 *            r(r‑1)/2 – 1. The polynomials u(n,x) and v(n,x) satisfy u(1,x) =
 *            1, v(1,x) = 1 u(n,x) = x*u(n‑1,x) + v(n‑1,x) v(n,x) =
 *            2x*u(n‑1,x) + x*v(n‑1,x) + 1 (see the OEIS entry for A209572)
 * Range:     u8 n <= 33, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Guard against an accidental zero index;  the OEIS offset is 1. */
    if (n == 0) return (seq_t)0;

    /* Find the triangle row r that contains the n‑th term. */
    idx_t r = 1;
    idx_t cum = 1;            /* cumulative count up to row r          */
    while (cum < n) {         /* increase r until the cumulative sum
                                 covers n                                 */
        ++r;
        cum += r;              /* r is the new row size                  */
    }

    /* Position of the desired coefficient within the r‑th row. */
    idx_t prev_cum = cum - r;          /* sum of rows 1 … r‑1                   */
    idx_t pos = n - prev_cum - 1;      /* zero‑based index inside row r         */

    /* Allocate memory for the coefficients of the previous row. */
    seq_t *u_prev;
    seq_t *v_prev;
#ifdef __cplusplus
    u_prev = new seq_t[r];
    v_prev = new seq_t[r];
#else
    u_prev = (seq_t *)malloc(sizeof(seq_t) * r);
    v_prev = (seq_t *)malloc(sizeof(seq_t) * r);
#endif

    /* Base case: row 1. */
    u_prev[0] = (seq_t)1;
    v_prev[0] = (seq_t)1;

    /* Build rows 2 … r.  At each step we construct new arrays
       of size k and then delete the old ones. */
    for (idx_t k = 2; k <= r; ++k) {
        seq_t *u_cur;
        seq_t *v_cur;
#ifdef __cplusplus
        u_cur = new seq_t[k];
        v_cur = new seq_t[k];
#else
        u_cur = (seq_t *)malloc(sizeof(seq_t) * k);
        v_cur = (seq_t *)malloc(sizeof(seq_t) * k);
#endif

        /* Constant term of u and v for the new row. */
        u_cur[0] = v_prev[0];
        v_cur[0] = (seq_t)1;

        /* Compute the remaining coefficients. */
        for (idx_t i = 1; i < k; ++i) {
            /* u(k,i) = u(k-1,i-1) + v(k-1,i)  (v(k-1,i) exists only
               for i ≤ k-2, otherwise it is 0). */
            seq_t add_u = u_prev[i - 1];
            seq_t add_v = (i <= k - 2) ? v_prev[i] : (seq_t)0;
            u_cur[i] = add_u + add_v;

            /* v(k,i) = 2 * u(k-1,i-1) + v(k-1,i-1). */
            v_cur[i] = (seq_t)2 * u_prev[i - 1] + v_prev[i - 1];
        }

        /* Release the memory of the previous row. */
#ifdef __cplusplus
        delete[] u_prev;
        delete[] v_prev;
#else
        free(u_prev);
        free(v_prev);
#endif

        /* Prepare for the next iteration. */
        u_prev = u_cur;
        v_prev = v_cur;
    }

    /* Retrieve the requested coefficient from the r‑th row and
       free the final arrays. */
    seq_t result = v_prev[pos];
#ifdef __cplusplus
    delete[] u_prev;
    delete[] v_prev;
#else
    free(u_prev);
    free(v_prev);
#endif
    return result;
}
