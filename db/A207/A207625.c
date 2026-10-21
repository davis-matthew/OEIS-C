/*
 * A207625  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A207626; see the Formula section.
 * https://oeis.org/A207625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   Recurrence for A207625 u(n,x)=u(n-1,x)+v(n-1,x),
 *            v(n,x)=2x*u(n-1,x)+(x+1)*v(n-1,x)+1, u(1,x)=1, v(1,x)=1. a(n) is
 *            the n‑th coefficient of the flattened triangle of the
 *            polynomials u(n,x). The sequence is indexed from 1
 * Range:     u8 n <= 24, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   row_of_terms, terms_before
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/row_of_terms.h"
#include "../utils/terms_before.h"

/* Return the n‑th term of A207625. */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined input */

    /* Determine the row that holds term n. */
    idx_t r = row_of_terms(n);

    /* Position within that row (0‑based). */
    idx_t cum_before = terms_before(r);
    idx_t pos = n - cum_before - 1;

    /* If row 1: only one term which is 1. */
    if (r == 1) {
        return (seq_t)1;
    }

    /* Allocate space for the polynomial of the previous row. */
    seq_t *u_prev = (seq_t *)malloc(sizeof(seq_t));
    seq_t *v_prev = (seq_t *)malloc(sizeof(seq_t));
    u_prev[0] = (seq_t)1; /* u(1,x)=1 */
    v_prev[0] = (seq_t)1; /* v(1,x)=1 */
    idx_t len_u_prev = 1;
    idx_t len_v_prev = 1;

    /* Build rows 2 .. r. */
    for (idx_t cur = 2; cur <= r; ++cur) {
        /* Lengths of the new row. */
        idx_t len_u_cur = (cur == 2) ? 1 : cur - 1; /* u(cur) */
        idx_t len_v_cur = cur;                      /* v(cur) */

        seq_t *u_cur = (seq_t *)malloc(len_u_cur * sizeof(seq_t));
        seq_t *v_cur = (seq_t *)malloc(len_v_cur * sizeof(seq_t));

        /* Initialize to zero. */
        for (idx_t i = 0; i < len_u_cur; ++i)
            u_cur[i] = (seq_t)0;
        for (idx_t i = 0; i < len_v_cur; ++i)
            v_cur[i] = (seq_t)0;

        /* v_cur = 2x*u_prev + (x+1)*v_prev + 1 */
        for (idx_t i = 0; i < len_u_prev; ++i)
            v_cur[i + 1] += (seq_t)2 * u_prev[i];

        for (idx_t i = 0; i < len_v_prev; ++i) {
            seq_t add = v_prev[i];
            v_cur[i]     += add; /* (x+1)*v_prev : + part */
            v_cur[i + 1] += add; /* (x+1)*v_prev : *x part */
        }
        v_cur[0] += (seq_t)1; /* +1 constant */

        /* u_cur = u_prev + v_prev */
        for (idx_t i = 0; i < len_u_prev; ++i)
            u_cur[i] += u_prev[i];
        for (idx_t i = 0; i < len_v_prev; ++i)
            u_cur[i] += v_prev[i];

        /* Free previous row and advance. */
        free(u_prev);
        free(v_prev);
        u_prev = u_cur;
        v_prev = v_cur;
        len_u_prev = len_u_cur;
        len_v_prev = len_v_cur;
    }

    /* u_prev now contains row r. */
    seq_t result = u_prev[pos];

    /* Clean up. */
    free(u_prev);
    free(v_prev);

    return result;
}
