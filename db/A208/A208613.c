/*
 * A208613  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A208612; see the Formula section.
 * https://oeis.org/A208613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for the triangle of coefficients of v(n,x):
 *   u(1,x) = 1,            v(1,x) = 1
 *   u(n,x) = u(n-1,x) + x*v(n-1,x)
 *   v(n,x) = (x+1)*u(n-1,x) + (x+1)*v(n-1,x) + 1
 *
 *   a(n) lists the coefficients of v(n,x) row by row, starting
 *   with a(1) = 1 for the first term.  Thus a(n) is obtained
 *   by flattening the triangle in row-major order.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find the row m containing the n-th coefficient:
     *  m is the smallest integer with m*(m+1)/2 >= n  */
    idx_t m = 1;
    while ((m * (m + 1) / 2) < n)
        ++m;

    /* Index k of the coefficient within that row (0‑based) */
    idx_t t = (m - 1) * m / 2;          /* sum of lengths of previous rows */
    idx_t k = n - t - 1;                /* 0‑based position in row m */

    /* Allocate space for two consecutive rows of u and v */
    size_t sz = (size_t)m;
    seq_t *prevU = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *prevV = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *curU  = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *curV  = (seq_t *)malloc(sz * sizeof(seq_t));

    /* Initial row 1 */
    prevU[0] = (seq_t)1;
    prevV[0] = (seq_t)1;

    /* Compute rows 2 .. m */
    for (idx_t i = 2; i <= m; ++i)
    {
        idx_t prevLen = i - 1;
        for (idx_t j = 0; j < i; ++j)
        {
            /* u_i[j] = u_{i-1}[j] + v_{i-1}[j-1] */
            seq_t term_u = (j < prevLen) ? prevU[j] : (seq_t)0;
            seq_t term_v_shift = (j > 0)     ? prevV[j - 1] : (seq_t)0;
            curU[j] = term_u + term_v_shift;

            /* v_i[j] = u_{i-1}[j] + u_{i-1}[j-1] +
                       v_{i-1}[j] + v_{i-1}[j-1] + (j==0?1:0) */
            seq_t u_part   = (j < prevLen) ? prevU[j] : (seq_t)0;
            seq_t u_shift  = (j > 0)     ? prevU[j - 1] : (seq_t)0;
            seq_t v_part   = (j < prevLen) ? prevV[j] : (seq_t)0;
            seq_t v_shift  = (j > 0)     ? prevV[j - 1] : (seq_t)0;
            seq_t const_term = (j == 0) ? (seq_t)1 : (seq_t)0;
            curV[j] = u_part + u_shift + v_part + v_shift + const_term;
        }

        /* Swap current and previous rows for next iteration */
        seq_t *tmpU = prevU; prevU = curU; curU = tmpU;
        seq_t *tmpV = prevV; prevV = curV; curV = tmpV;
    }

    /* Result is the k‑th coefficient in the final row (prevV) */
    seq_t result = prevV[(size_t)k];

    /* Clean up */
    free(prevU);
    free(prevV);
    free(curU);
    free(curV);

    return result;
}
