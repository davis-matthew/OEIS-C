/*
 * A145364  Lower triangular array, called S1hat(-2), related to partition
 *          number array A145363.
 * https://oeis.org/A145364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   For n>=1, let r be the row number such that r(r-1)/2 < n <=
 *            r(r+1)/2 (i.e. the offset used in OEIS lower‑triangular arrays).
 *            Let c = n - r(r-1)/2 (column index, 1 ≤ c ≤ r). a(n) = Σ 2^{k2 +
 *            k3} over all non‑negative integer solutions (k1, k2, k3)
 *            satisfying k1 + k2 + k3 = c (number of parts) k1 +...
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: pow2() is provided by the utilities library. */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Find row r: smallest r such that r(r+1)/2 >= n */
    idx_t lo = 1, hi = 4294967296ULL; /* 2^32, upper bound for r when n < 2^64 */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        idx_t val = (mid & 1) ? mid * ((mid + 1) / 2)
                              : (mid / 2) * (mid + 1);
        if (val >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;

    /* Compute starting index of row r: r(r-1)/2 */
    idx_t start = (r & 1) ? ((r - 1) / 2) * r
                          : (r / 2) * (r - 1);
    /* Column index (1‑based) */
    idx_t c = n - start;

    /* Compute s = r - c */
    if (c > r) return 0; /* safety, should not occur for n >= 1 */
    idx_t s = r - c;

    /* Compute bounds for k3 */
    idx_t L = (s > c) ? (s - c) : 0;
    idx_t U = s / 2;

    if (L > U)
        return 0;

    /* a(n) = 2^{s-L+1} - 2^{s-U} */
    seq_t term_high = pow2((s - L) + 1);
    seq_t term_low  = pow2(s - U);
    return term_high - term_low;
}
