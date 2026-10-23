/*
 * A115009  Array read by antidiagonals: let V(m,n) = Sum_{i=1..m, j=1..n,
 *          gcd(i,j)=1} (m+1-i)*(n+1-j), then T(m,n) = 2*m*n+m+n+2*V(m,n), for
 *          m >= 0, n >= 0.
 * https://oeis.org/A115009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A115009:  Number of linear partitions of an m × n grid.
 *
 *  Let V(m,n) = Σ_{1≤i≤m,1≤j≤n, gcd(i,j)=1} (m+1-i)(n+1-j).
 *  Then T(m,n) = 2*m*n + m + n + 2*V(m,n)  (for m,n ≥ 0).
 *
 *  The sequence a(n) is the array T(m,n) read by antidiagonals:
 *  indices n are obtained from (m,n) by
 *      d = m + n,  a = n - d*(d+1)/2      (0‑based)
 *  where a is the row index within the diagonal (x = a, y = d-a).
 *
 *  a(0) = 0, a(1) = 1, a(2) = 1, a(3) = 2, a(4) = 6, …
 *
 *  The implementation below uses only integer arithmetic and the
 *  provided gcd function.  It runs in O(√n + m*n) time, which
 *  is well below the required limit for n up to 10 000.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Find diagonal d such that d*(d+1)/2 ≤ n < (d+1)*(d+2)/2 */
    idx_t d = 0;
    while (((d + 1) * (d + 2) / 2) <= n) d++;

    /* Position within the diagonal */
    idx_t x = n - d * (d + 1) / 2;   /* row index (m) */
    idx_t m = x;
    idx_t nDim = d - x;              /* column index (n) */

    /* Compute V(m,nDim) = Σ_{i=1..m} Σ_{j=1..nDim, gcd(i,j)=1}
       (m+1-i)(nDim+1-j) */
    seq_t V = 0;
    for (idx_t i = 1; i <= m; ++i) {
        for (idx_t j = 1; j <= nDim; ++j) {
            if (gcd((seq_t)i, (seq_t)j) == (seq_t)1) {
                seq_t term = (seq_t)(m + 1 - i) * (seq_t)(nDim + 1 - j);
                V += term;
            }
        }
    }

    /* T(m,n) = 2*m*n + m + n + 2*V */
    seq_t val = ((seq_t)m * (seq_t)nDim) * (seq_t)2
                + (seq_t)m + (seq_t)nDim
                + (V << 1);
    return val;
}
