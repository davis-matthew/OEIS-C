/*
 * A210214  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A210213; see the Formula section.
 * https://oeis.org/A210214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle of coefficients of v(n,x) (A210214).
   Recurrence:
     u(1,x) = v(1,x) = 1
     u(n,x) = x*u(n-1,x) + v(n-1,x) + 1
     v(n,x) = u(n-1,x) + (x+1)*v(n-1,x) + 1   (n > 1)
   a(n) is the n‑th entry of the triangle formed by the
   coefficients of v(n,x) read row‑by‑row (row r has r terms,
   numbered from the constant term).
   This implementation builds the required row r and returns
   the appropriate coefficient.  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find the row r such that T(r-1) < n ≤ T(r), where
       T(k) = k*(k+1)/2.  */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) {
        ++r;
    }
    /* Size needed to hold the largest row. */
    idx_t maxlen = r;

    /* Allocate two pairs of arrays: current and previous rows. */
    seq_t *U_prev = (seq_t *)malloc(maxlen * sizeof(seq_t));
    seq_t *V_prev = (seq_t *)malloc(maxlen * sizeof(seq_t));
    seq_t *U_curr = (seq_t *)malloc(maxlen * sizeof(seq_t));
    seq_t *V_curr = (seq_t *)malloc(maxlen * sizeof(seq_t));

    if (!U_prev || !V_prev || !U_curr || !V_curr) {
        /* Allocation failed – the caller must handle this. */
        return (seq_t)0;
    }

    /* Initialize the base case: row 1. */
    U_prev[0] = (seq_t)1;
    V_prev[0] = (seq_t)1;
    size_t prev_len = 1;

    /* Build rows 2 … r. */
    for (idx_t curr = 2; curr <= r; ++curr) {
        size_t curr_len = (size_t)curr;

        /* Constant terms. */
        U_curr[0] = V_prev[0] + (seq_t)1;
        V_curr[0] = U_prev[0] + V_prev[0] + (seq_t)1;

        for (size_t i = 1; i < curr_len; ++i) {
            /* u coefficient of degree i. */
            seq_t ui = 0;
            if (i - 1 < prev_len) ui += U_prev[i - 1];
            if (i < prev_len) ui += V_prev[i];
            U_curr[i] = ui;

            /* v coefficient of degree i. */
            seq_t vi = 0;
            if (i < prev_len) vi += U_prev[i];
            if (i - 1 < prev_len) vi += V_prev[i - 1];
            if (i < prev_len) vi += V_prev[i];
            V_curr[i] = vi;
        }

        /* Swap prev ↔ curr for next iteration. */
        seq_t *tmp = U_prev; U_prev = U_curr; U_curr = tmp;
        tmp = V_prev; V_prev = V_curr; V_curr = tmp;
        prev_len = curr_len;
    }

    /* Column index (0‑based) within the found row. */
    idx_t col0 = n - ((r - 1) * r) / 2 - 1;

    seq_t result = V_prev[col0];

    /* Free all allocated memory. */
    free(U_prev);
    free(V_prev);
    free(U_curr);
    free(V_curr);

    return result;
}
