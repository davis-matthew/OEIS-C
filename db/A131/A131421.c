/*
 * A131421  Triangle read by rows (n>=1, 1<=k<=n): T(n,k) = 2*(n+k) - 3.
 * https://oeis.org/A131421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 2*(r + k) - 3 where r is the smallest integer such that
 *            r(r+1)/2 >= n, and k = n - r(r-1)/2 (the column index within the
 *            row)
 * Range:     u8 n <= 7, u16 n <= 1439, u32 n <= 9434, u64 n <= 9434,
 *            u128 n <= 9434, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* not defined for n < 1, but guard against bad input */

    /* Compute the floor of sqrt(1 + 8*n).  The result fits in idx_t. */
    seq_t s = floor_sqrt((seq_t)1 + (seq_t)8 * n);
    idx_t r = (idx_t)(s - 1) / 2;               /* floor estimate of row */

    /* Ensure r satisfies r(r+1)/2 >= n */
    seq_t tri = (seq_t)r * (r + 1) / 2;
    if (tri < n) ++r;

    /* Previous triangular number: (r-1)*r/2 */
    seq_t prev = (seq_t)(r - 1) * r / 2;

    idx_t k = (idx_t)(n - prev);   /* column index (1 ≤ k ≤ r) */

    return 2 * ((seq_t)r + (seq_t)k) - 3;
}
