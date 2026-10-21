/*
 * A209699  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A209700; see the Formula section.
 * https://oeis.org/A209699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A209699:  Triangle of coefficients u(n,i) of the polynomials
    u(n,x) defined by
        u(1,x)=1 ,  v(1,x)=1
        u(n,x)=x*u(n-1,x)+x*v(n-1,x)
        v(n,x)=2x*u(n-1,x)+v(n-1,x)+1
    The row r of the triangle has r coefficients; flattening the rows
    (row 1, row 2, …) yields the sequence a(1),a(2),…      */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Return the n-th term of A209699 (1‑based index). */
seq_t a(idx_t n)
{
    /* Find the row r containing position n.
       T(r) = r*(r+1)/2 is the number of terms up to row r.        */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    /* The index of the first element of this row (0‑based). */
    idx_t start = ((r - 1) * r) / 2;

    /* Column of the desired element within the row (0‑based). */
    idx_t col = n - start - 1;

    /* Special case: the very first element. */
    if (r == 1)
        return (seq_t)1;

    /* Allocate arrays for the previous and current rows. */
    seq_t *U_prev = (seq_t *)malloc(r * sizeof(seq_t));
    if (!U_prev) return (seq_t)0;          /* allocation failure: return 0 */
    seq_t *V_prev = (seq_t *)malloc(r * sizeof(seq_t));
    if (!V_prev) { free(U_prev); return (seq_t)0; }

    /* Initialize row 1. */
    U_prev[0] = (seq_t)1;
    V_prev[0] = (seq_t)1;

    /* Build rows 2 … r. */
    for (idx_t k = 2; k <= r; ++k)
    {
        seq_t *U_new = (seq_t *)malloc(k * sizeof(seq_t));
        seq_t *V_new = (seq_t *)malloc(k * sizeof(seq_t));
        if (!U_new || !V_new) {
            free(U_prev); free(V_prev);
            if (U_new) free(U_new);
            if (V_new) free(V_new);
            return (seq_t)0;
        }

        /* New row parameters. */
        U_new[0] = (seq_t)0;
        V_new[0] = V_prev[0] + (seq_t)1;

        /* Remaining coefficients. */
        for (idx_t i = 1; i < k; ++i)
        {
            seq_t u_prev_im1 = U_prev[i - 1];
            seq_t v_prev_im1 = V_prev[i - 1];
            U_new[i] = u_prev_im1 + v_prev_im1;                 /* x*(u+v) */

            seq_t v_prev_i = (i < (k - 1)) ? V_prev[i] : (seq_t)0;
            V_new[i] = (seq_t)2 * u_prev_im1 + v_prev_i;        /* 2x*u + v */
        }

        /* Free the previous row arrays. */
        free(U_prev);
        free(V_prev);

        /* Promote the new row to be the previous row. */
        U_prev = U_new;
        V_prev = V_new;
    }

    /* The desired value is in the column position. */
    seq_t result = U_prev[col];

    /* Clean up. */
    free(U_prev);
    free(V_prev);

    return result;
}
