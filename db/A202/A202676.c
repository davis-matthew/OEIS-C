/*
 * A202676  Symmetric matrix based on (1,4,7,10,13,...), by antidiagonals.
 * https://oeis.org/A202676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = M(i,j) where (i,j) are obtained from the antidiagonal
 *            enumeration of the infinite matrix M = T'*T with s(n)=1+3(n-1);
 *            M(i,j) = Σ_{k=1}^{min(i,j)} (3i-3k+1)(3j-3k+1)
 * Range:     u8 n <= 39, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   min_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stddef.h>
#include "../utils/min_idx.h"

seq_t a(idx_t n){
    /* Find diagonal index d such that d(d+1)/2 >= n */
    double t = sqrt((double)8 * (double)n + 1.0);
    idx_t d = (idx_t)((t - 1.0) / 2.0);
    if ( d*(d+1)/2 < n ) ++d;
    /* Position within the diagonal */
    idx_t prev = d*(d-1)/2;
    idx_t offset = n - prev - 1;           /* 0‑based */
    idx_t i = offset + 1;                   /* row */
    idx_t j = d - offset;                   /* column */
    seq_t a_i = (seq_t)i;
    seq_t b_j = (seq_t)j;
    seq_t m = (seq_t)min_idx(i,j);
    seq_t uv = 9*a_i*b_j + 3*a_i + 3*b_j + 1;
    seq_t term1 = m * uv;
    seq_t term2 = ( (9*(a_i + b_j) + 6) * m * (m + 1) ) / 2;
    seq_t term3 = ( 3 * m * (m + 1) * (2*m + 1) ) / 2;
    return term1 - term2 + term3;
}
