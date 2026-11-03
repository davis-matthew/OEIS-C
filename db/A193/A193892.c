/*
 * A193892  Mirror of the triangle A193891.
 * https://oeis.org/A193892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 63 (2026-11-01)
 * Helpers:   triangular
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A193892: Mirror of triangle A193891
   a(0) = 1
   a(n) = w(r, k) where r = floor((sqrt(8n+1)-1)/2),
          k = offset(r) + r - n,
          offset(r) = r*(r+1)/2,
          w(r, k) = [ 6r + k*( 3(r-1)k + (9r-2) - k^2 ) ] / 6     (r ≥ 1)
   For r = 0, w(0,0) = 1. 
   The formula for w(r,k) was derived by solving the cubic
   interpolation from the boundary values
      w(r,0)=r,   w(r,r)=r(r+1)(r+2)/3,
      w(r,r-1)=w(r,r)-r(r+1)/2,   w(r,r-2)=w(r,r)-r(r+1).
*/

#include <math.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/triangular.h"

/* Compute w(r, k) for r >= 1 and 0 <= k <= r using only positive arithmetic. */
static seq_t w_value(idx_t r, idx_t k) {
    if (r == 0)      /* w(0,0) = 1 */
        return (seq_t)1;

    /* All intermediate values fit in seq_t if seq_t is uint64_t or larger. */
    seq_t s1 = r;                  /* r */
    seq_t s2 = k;                   /* k */

    /* inner = 3*(r-1)*k + (9*r-2) - k^2  (always non‑negative for r>=1) */
    seq_t inner = 3 * (s1 - 1) * s2 + (9 * s1 - 2) - s2 * s2;

    /* numerator = 6*r + k * inner */
    seq_t num = 6 * s1 + s2 * inner;

    /* exact division by 6 */
    return num / 6;
}

/* Compute the offset where row r starts (0-based). */

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0)     /* a(0) = 1 */
        return (seq_t)1;

    /* Find r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t r;
    double tmp = sqrt(8.0 * (double)n + 1.0);
    r = (idx_t)((tmp - 1.0) / 2.0);
    /* Adjust r to guarantee monotonicity (in case of floating‑precision errors). */
    while ((r + 1) * (r + 2) / 2 <= n) r++;
    while (r * (r + 1) / 2 > n) r--;

    /* Compute reversed position within the row. */
    idx_t offset = triangular(r);
    idx_t k_rev = offset + r - n;          /* 0 <= k_rev <= r */

    /* Return triangle entry w(r, k_rev). */
    return w_value(r, k_rev);
}
