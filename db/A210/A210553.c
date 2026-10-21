/*
 * A210553  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A210552; see the Formula section.
 * https://oeis.org/A210553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence :  u(1,x)=1 , v(1,x)=1
 *               u(n,x)=x*u(n-1,x)+x*v(n-1,x)+1
 *               v(n,x)=x*u(n-1,x)+v(n-1,x)+1
 * The flattened triangle of the coefficients of v(n,x) is the sequence A210553.  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Determine the row r that contains the nth term.  The number of
     * terms up to row r is r*(r+1)/2. */
    idx_t r = 1;
    idx_t cum = 1;          /* terms up to current row */
    while (cum < n) {
        ++r;
        cum += r;
    }

    /* 0‑based index within the row. */
    idx_t j = n - (cum - r) - 1;

    /* Allocate arrays for the current row.  The maximum size needed is r. */
    seq_t *u = (seq_t *)malloc(r * sizeof(seq_t));
    seq_t *v = (seq_t *)malloc(r * sizeof(seq_t));
    if (!u || !v) {
        free(u);
        free(v);
        return (seq_t)0;
    }

    /* Base case: row 1. */
    u[0] = (seq_t)1;
    v[0] = (seq_t)1;

    /* Build rows 2..r. */
    for (idx_t i = 2; i <= r; ++i) {
        idx_t len_prev = i - 1;             /* length of previous row */

        seq_t *newu = (seq_t *)malloc(i * sizeof(seq_t));
        seq_t *newv = (seq_t *)malloc(i * sizeof(seq_t));
        if (!newu || !newv) {
            free(u);
            free(v);
            free(newu);
            free(newv);
            return (seq_t)0;
        }

        /* Constant terms. */
        newu[0] = (seq_t)1;
        newv[0] = v[0] + (seq_t)1;

        /* Coefficients for degrees 1 .. i-1. */
        for (idx_t k = 1; k < i; ++k) {
            if (k < len_prev) {
                /* k ≤ i-2 : vprev[k] exists */
                newu[k] = u[k-1] + v[k-1];
                newv[k] = v[k] + u[k-1];
            } else { /* k == i-1 : vprev[k] does not exist */
                newu[k] = u[k-1] + v[k-1];
                newv[k] = u[k-1];
            }
        }

        free(u);
        free(v);
        u = newu;
        v = newv;
    }

    seq_t result = v[j];

    free(u);
    free(v);
    return result;
}
