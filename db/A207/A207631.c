/*
 * A207631  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A207632; see the Formula section.
 * https://oeis.org/A207631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   triangle of coefficients of polynomials u(n,x) defined by *
 *            u(1,x)=1, v(1,x)=1; * u(n,x)=u(n-1,x)+v(n-1,x); *
 *            v(n,x)=(x+1)*u(n-1,x)+x*v(n-1,x)+1. * a(n) is the n‑th
 *            coefficient read row by row (1‑based)
 * Range:     u8 n <= 30, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the n‑th term of OEIS sequence A207631. */
seq_t a(idx_t n)
{
    /* Handle offset 1 explicitly. */
    if (n == (idx_t)1)
        return (seq_t)1;

    /* Find the row r (≥2) that contains the n‑th term. */
    idx_t r = 1;              /* current row number */
    size_t total = 1;         /* terms up to current row */
    while (total < (size_t)n) {
        ++r;
        total += (size_t)(r - 1);
    }

    /* Number of coeffs in the found row and index of the desired coeff. */
    size_t row_len      = (size_t)(r - 1);          /* row has r-1 coeffs   */
    size_t total_before = total - row_len;         /* terms before this row */
    size_t j             = (size_t)n - total_before - 1;  /* 0‑based index */

    /* Allocate buffers for the two most recent rows of u and v. */
    size_t max_row = (size_t)r;
    seq_t *u_prev = (seq_t *)calloc(max_row,          sizeof(seq_t));
    seq_t *u_cur  = (seq_t *)calloc(max_row,          sizeof(seq_t));
    seq_t *v_prev = (seq_t *)calloc(max_row + 1,      sizeof(seq_t));
    seq_t *v_cur  = (seq_t *)calloc(max_row + 1,      sizeof(seq_t));

    /* Initial values for row 1. */
    u_prev[0] = (seq_t)1;
    v_prev[0] = (seq_t)1;
    size_t len_u_prev = 1;
    size_t len_v_prev = 1;

    /* Build rows up to r using the recurrences. */
    for (idx_t row = 2; row <= r; ++row) {
        size_t len_u_new = (size_t)(row - 1);
        size_t len_v_new = (size_t)row;

        /* u(row) = u(prev) + v(prev) */
        for (size_t k = 0; k < len_u_new; ++k) {
            seq_t val = 0;
            if (k < len_u_prev) val += u_prev[k];
            if (k < len_v_prev) val += v_prev[k];
            u_cur[k] = val;
        }

        /* v(row) = (x+1)u(prev) + x*v(prev) + 1 */
        for (size_t k = 0; k < len_v_new; ++k) {
            seq_t val = 0;
            if (k < len_u_prev)          val += u_prev[k];
            if (k > 0 && (k - 1) < len_u_prev) val += u_prev[k - 1];
            if (k > 0 && (k - 1) < len_v_prev) val += v_prev[k - 1];
            if (k == 0)                  val += (seq_t)1;
            v_cur[k] = val;
        }

        /* Swap current and previous buffers for next iteration. */
        seq_t *tmp;
        tmp = u_prev; u_prev = u_cur; u_cur = tmp;
        tmp = v_prev; v_prev = v_cur; v_cur = tmp;

        len_u_prev = len_u_new;
        len_v_prev = len_v_new;
    }

    /* Retrieve requested coefficient from the final row. */
    seq_t result = u_prev[j];

    /* Clean up. */
    free(u_cur);
    free(v_cur);
    free(u_prev);
    free(v_prev);

    return result;
}
