/*
 * A159016  Square array A, read by antidiagonals, with entry n in row k given
 *          by A(k,0) = A028387(k) and A(k,n) = A(k,n-1) +
 *          floor(sqrt(A(k,n-1))), n>0, k>=0.
 * https://oeis.org/A159016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A028387(k) = k*(k+3)+1 A(k,0) = A028387(k) A(k,n) = A(k,n-1) +
 *            floor(sqrt(A(k,n-1))), n>0 a(n) = A( m, d-m ) where d is the
 *            unique integer with d*(d+1)/2 ≤ n < (d+1)*(d+2)/2 m = n -
 *            d*(d+1)/2 (row index)
 * Range:     u8 n <= 42, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    /* Find the diagonal index d (0‑based) such that
       T(d) = d*(d+1)/2 ≤ n < T(d+1) = (d+1)*(d+2)/2. */
    idx_t d = 0;
    while ((d * (d + 1)) / 2 <= n) {
        d++;
    }
    d--;                         /* now T(d) ≤ n */
    idx_t tri = (d * (d + 1)) / 2;
    idx_t m = n - tri;           /* offset in the diagonal */
    idx_t row = m;
    idx_t col = d - m;           /* column index in the row */

    /* Row base value A(k,0) */
    seq_t val = (seq_t)row * ((seq_t)row + 3) + 1;  /* k*(k+3)+1 */

    /* Advance 'col' steps using the recurrence. */
    while (col-- > 0) {
        val += floor_sqrt(val);
    }

    return val;
}
