/*
 * A136329  Triangular sequence of coefficients of a polynomial recursion for
 *          C_n and B_n Cartan matrices: p(x, n) = (-2 + x)*p(x, n - 1) - p(x,
 *          n - 2) p(x,n)=x2-4*x+4-m:m=4;(related sequence:
 *          A_n:m=1,G_n,m=3,B_n,C_n,m=2) This triangular sequence is an
 *          extension to the Cartan pattern of matrices.
 * https://oeis.org/A136329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, uned, sign
 * Formula:   not stated
 * Range:     i8 n <= 31, i16 n <= 66, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A136329
    a(n) is the flattened list of the coefficients of the polynomials
    p_0(x)=4 ,  p_1(x)=x-2 and   p_n(x)=(x-2)p_{n-1}(x)-p_{n-2}(x).
    The coefficient list of each p_k is written from constant term upwards
    and the rows are concatenated in order.
    Note: a(1) is defined as 1 (the OEIS first term). */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 1) return 1;   /* special case for the OEIS offset */

    /* Find the row containing the n-th term */
    idx_t t   = n;                 /* 1‑based index within the flattened array */
    idx_t row = 1;                 /* minimal r such that r(r+1)/2 >= t   */
    while ((row * (row + 1)) / 2 < t) row++;

    idx_t r_len = row;             /* degree of polynomial p_{row-1} + 1 */
    idx_t k     = t - ((row - 1) * row) / 2 - 1;   /* index inside the row */

    /* Allocate temporary arrays.  Size r_len+2 is enough for all polynomials up
       to degree r_len-1. */
    idx_t sz   = r_len + 2;
    seq_t *prevprev = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *prev     = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *curr     = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!prevprev || !prev || !curr) return 0;   /* allocation failure */

    /* Initialise p_0 and p_1 */
    for (idx_t i = 0; i < sz; ++i) {
        prevprev[i] = 0;
        prev[i]     = 0;
        curr[i]     = 0;
    }
    prevprev[0] = 4;           /* p_0(x)=4 */
    prev[0]   = -2;            /* constant term of p_1(x)=x-2 */
    prev[1]   =  1;            /* coefficient of x */

    /* Compute subsequent polynomials up to p_{row-1} */
    for (idx_t idx = 2; idx < r_len; ++idx) {
        /* curr = (x-2)*prev  */
        for (idx_t j = 0; j < sz; ++j) curr[j] = 0;
        idx_t prev_len = idx + 1;          /* length of prev polynomial */
        for (idx_t j = 0; j < prev_len; ++j) {
            curr[j]   += prev[j] * ((seq_t)(-2));
            curr[j + 1] += prev[j];
        }
        /* curr -= prevprev */
        idx_t prevprev_len = idx - 1;      /* length of prevprev polynomial */
        for (idx_t j = 0; j < prevprev_len; ++j)
            curr[j] -= prevprev[j];

        /* Rotate buffers: prevprev <- prev ; prev <- curr ; curr reused */
        seq_t *tmp = prevprev;
        prevprev   = prev;
        prev       = curr;
        curr       = tmp;
    }

    /* Result is the k-th coefficient of the polynomial in 'prev' */
    seq_t result = prev[k];

    free(prevprev);
    free(prev);
    free(curr);

    return result;
}
