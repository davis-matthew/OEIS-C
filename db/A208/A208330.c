/*
 * A208330  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A208331; see the Formula section.
 * https://oeis.org/A208330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A001045(k+1) * binomial(n-1,k) where n ≥ 1, r is the
 *            smallest integer with r(r+1)/2 ≥ n, k = n - r(r-1)/2 - 1 (0 ≤ k
 *            < r). T(r,k) = Jacobsthal(k+1) * C(r-1,k). Initial values:
 *            a(1)=1, a(2)=1, a(3)=1, a(4)=1, a(5)=2, ... The sequence is
 *            produced by listing the triangle row‑wise
 * Range:     u8 n <= 32, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   binom_value, jacob
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom_value.h"
#include "../utils/jacob.h"

/* Integer floor square root for idx_t (uint64_t). */
static idx_t isqrt_u64(idx_t v) {
    idx_t high = v;
    idx_t low = 0;
    idx_t mid, r = 0;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        if (mid == 0) {
            r = 0;
            break;
        }
        if (mid > v / mid)
            high = mid - 1;
        else {
            r = mid;
            low = mid + 1;
        }
    }
    return r;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)0;   /* Offset is 1, unused for n=0 */

    /* Find minimal r with r(r+1)/2 ≥ n. */
    idx_t r = (isqrt_u64((idx_t)8 * n + (idx_t)1) - (idx_t)1) / (idx_t)2;
    if (r * (r + (idx_t)1) / (idx_t)2 < n) r++;

    /* Number of terms before row r. */
    idx_t prev = (r - (idx_t)1) * r / (idx_t)2;

    /* Column index within the row. */
    idx_t k = n - prev - (idx_t)1;

    /* Jacobsthal number J(k+1). */
    seq_t J = jacob(k + (idx_t)1);

    /* Binomial coefficient C(r-1, k). */
    seq_t bin = binom_value(r - (idx_t)1, k);

    /* Triangle value T(r,k). */
    return J * bin;
}
