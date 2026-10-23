/*
 * A166124  Triangle, read by rows, given by [0,1/2,1/2,0,0,0,0,0,0,0,...]
 *          DELTA [2,-1,0,0,0,0,0,0,0,0,...] where DELTA is the operator
 *          defined in A084938.
 * https://oeis.org/A166124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 0 when n > 0 is a triangular number; otherwise a(n) = 1
 *            for all indices in each block except the last index before the
 *            next triangular number, where a(n) = 2. Initial values: a(0)=1,
 *            a(1)=0, a(2)=2
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/int_sqrt.h"


/* Computes a(n) for OEIS sequence A166124.  */
seq_t a(idx_t n)
{
    /* Special case for the first term. */
    if (n == 0)
        return 1;

    /* Compute floor(sqrt(1 + 8*n)).  */
    idx_t rad = 1 + n * 8;
    idx_t t   = int_sqrt(rad);                 /* ⌊√(1+8n)⌋          */
    idx_t m0  = (t - 1) / 2;                   /* ⌊(-1+√(1+8n))/2⌋   */

    /* Triangular number T(m0) = m0*(m0+1)/2.  */
    idx_t tri = m0 * (m0 + 1) / 2;
    if (tri == n)                /* n is triangular (>0) */
        return 0;                /* all triangular numbers give 0 */

    /* n is not triangular.  Let m = m0+1 so that T(m) > n.  */
    idx_t m = m0 + 1;

    /* Previous triangular number T(m-1) = (m-1)*m/2.  */
    idx_t prev_tri = (m - 1) * m / 2;

    /* Offset of n within its block, 0‑based.  */
    idx_t offset = n - prev_tri - 1;

    /* In the block the last element (offset == m-2) is 2,
       all earlier elements are 1.  */
    if (offset < (m - 2))
        return 1;
    else
        return 2;
}
