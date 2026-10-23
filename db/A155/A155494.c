/*
 * A155494  Triangle T(n, k) = (k+1)*(n-k+1)*binomial(n,k) with T(n, 0) = T(n,
 *          n) = 1, read by rows.
 * https://oeis.org/A155494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 95, u32 n <= 416, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle T(r,k) = (k+1)*(r-k+1)*binomial(r,k) with T(r,0)=T(r,r)=1,
 * read by rows.  The sequence a(n) is the row‑wise enumeration of this
 * triangle; a(n) = T(r,k) where n = r*(r+1)/2 + k, 0 ≤ k ≤ r. */
#include <stdint.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2.
       A binary search over r is sufficient (r will never exceed ~140
       for n <= 10000). */
    idx_t low = 0;
    idx_t high = 65535;           /* large enough upper bound for n up to 10^9 */
    idx_t r = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t tri = (mid * (mid + 1)) >> 1;   /* mid*(mid+1)/2 */
        if (tri <= n) {
            r = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    /* Compute linear offset of the first element of row r. */
    idx_t offset = (r * (r + 1)) >> 1;
    idx_t k = n - offset;   /* column index within the row (0 ≤ k ≤ r) */

    /* Boundary cases: first and last elements are 1. */
    if (k == 0 || k == r)
        return (seq_t)1;

    /* Compute T(r,k) = (k+1)*(r-k+1)*binomial(r,k). */
    seq_t bin = binomial(r, k);
    seq_t val = bin * (seq_t)(k + 1);
    val *= (seq_t)(r - k + 1);
    return val;
}
