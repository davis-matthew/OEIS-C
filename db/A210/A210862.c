/*
 * A210862  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A210863; see the Formula section.
 * https://oeis.org/A210862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the flattened expansion of the triangle of coefficients
 *            of the polynomials u(n,x) defined by u(1,x) = 1 v(1,x) = 1
 *            u(n,x) = u(n-1,x) + x·v(n-1,x) + 1 v(n,x) = (x + n – 1)·u(n-1,x)
 *            + x·v(n-1,x) (n > 1) The triangle’s a(n) term is the column‑wise
 *            reading of the rows in which the first eleme...
 * Range:     u8 n <= 30, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes a(n) for the OEIS sequence A210862.  */
seq_t a(idx_t n)
{
    /* Defensive guard for undefined n < 1, not needed in the normal
       sequence but keeps the function well defined. */
    if (n < 1) {
        return (seq_t)0;
    }

    /* Determine the row (r) that contains the n‑th term.  */
    size_t r = 1;
    size_t cumulative = 1;                     /* 1 + 2 + ... + r */
    while (cumulative < (size_t)n) {
        ++r;
        cumulative += r;
    }

    /* Column index (0‑based) within the row.  */
    size_t prev_total = (r - 1) * r / 2;        /* number of terms before row r */
    size_t col = (size_t)n - prev_total - 1;   /* 0‑based column */

    /* Handle the first row explicitly.  */
    if (r == 1) {
        /* Row 1 contains only the constant term of u(1,x) which is 1. */
        return (seq_t)1;
    }

    /* Allocate arrays for the previous row (row 1).  */
    size_t prev_len = 1;
    seq_t *prev_u = (seq_t*)malloc(prev_len * sizeof(seq_t));
    seq_t *prev_v = (seq_t*)malloc(prev_len * sizeof(seq_t));
    prev_u[0] = (seq_t)1;
    prev_v[0] = (seq_t)1;

    /* Build rows 2 through r.  */
    for (size_t row = 2; row <= r; ++row) {
        size_t curr_len = row;
        seq_t *curr_u = (seq_t*)malloc(curr_len * sizeof(seq_t));
        seq_t *curr_v = (seq_t*)malloc(curr_len * sizeof(seq_t));

        /* k = 0 (constant term).  */
        curr_u[0] = prev_u[0] + (seq_t)1;
        curr_v[0] = (seq_t)(row - 1) * prev_u[0];

        /* k >= 1 */
        for (size_t k = 1; k < curr_len; ++k) {
            seq_t u_prev_k     = (k < prev_len ? prev_u[k]     : (seq_t)0);
            seq_t v_prev_km1   = (k > 0       ? prev_v[k - 1] : (seq_t)0);
            seq_t u_prev_km1   = (k > 0       ? prev_u[k - 1] : (seq_t)0);

            /* u(n,x) coefficient recurrence: u_k = u_{k}^{prev} + v_{k-1}^{prev} */
            curr_u[k] = u_prev_k + v_prev_km1;

            /* v(n,x) coefficient recurrence:
               v_k = (n-1)*u_k^{prev} + u_{k-1}^{prev} + v_{k-1}^{prev} */
            seq_t term1 = (seq_t)(row - 1) * u_prev_k;
            seq_t term2 = u_prev_km1;
            seq_t term3 = v_prev_km1;
            curr_v[k] = term1 + term2 + term3;
        }

        /* Free the previous row and advance.  */
        free(prev_u);
        free(prev_v);
        prev_u   = curr_u;
        prev_v   = curr_v;
        prev_len = curr_len;
    }

    /* Retrieve the required coefficient and free memory.  */
    seq_t result = prev_u[col];
    free(prev_u);
    free(prev_v);
    return result;
}
