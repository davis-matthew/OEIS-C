/*
 * A127803  Inverse of number triangle A(n,k) = 1/(2*2^n-1) if k <= n <= 2k, 0
 *          otherwise.
 * https://oeis.org/A127803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = B[r][c] where B is the inverse of the triangle * A(n,k) =
 *            1/(2*2^n-1) if k <= n <= 2k, 0 otherwise. * The values are
 *            computed by the recurrence * * Let D[i] = 2*2^i - 1 = 2^(i+1) -
 *            1. * For each row r, define c[r] = 1. * For i = r-1 ... 0: *
 *            c[i] = - Σ_{j=i+1}^{min(2i,r)} c[j] * Then B[r][...
 * Range:     i8 n <= 34, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Find row r such that T(r) <= n < T(r+1), where T(r) = r*(r+1)/2. */
    idx_t low = 0;
    idx_t high = 1;
    /* exponential search to find an upper bound */
    while (( (__int128)high * (high + 1) / 2 ) <= n) {
        high <<= 1;
    }
    /* binary search between low and high */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if ( ( (__int128)mid * (mid + 1) / 2 ) <= n )
            low = mid;
        else
            high = mid;
    }
    idx_t r = low;                 /* row index */
    idx_t row_start = r * (r + 1) / 2;   /* first index of this row */
    idx_t c = n - row_start;        /* column index within the row */

    /* Allocate coefficient array for the row */
    seq_t *coef = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    if (!coef) return 0;            /* memory failure → return 0 (unlikely) */

    /* Recurrence to fill coef[i] : c[i] values */
    coef[r] = 1;                    /* base case */
    for (idx_t i = r; i-- > 0;) {   /* i from r-1 down to 0 */
        seq_t sum = 0;
        idx_t jmax = (2 * i <= r) ? 2 * i : r;
        for (idx_t j = i + 1; j <= jmax; ++j) {
            sum += coef[j];
        }
        coef[i] = -sum;
    }

    /* Compute the answer: B[r][c] = coef[c] * (2^(c+1) - 1) */
    seq_t D_c = pow2(c + 1);
    D_c = D_c - 1;
    seq_t result = coef[c] * D_c;

    free(coef);
    return result;
}
