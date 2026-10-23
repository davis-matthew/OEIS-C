/*
 * A193961  Triangular array: the self-fusion of (p(n,x)), where
 *          p(n,x)=sum{((k+1)^2)*x^(n-k) : 0<=k<=n}.
 * https://oeis.org/A193961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Let n index the array row‑major, starting at 0. Find the unique
 *            integers i,j with i*(i+1)/2 ≤ n < (i+1)*(i+2)/2 (row i) j = n -
 *            i*(i+1)/2 (column j, 0 ≤ j ≤ i) Then for i > 0 a(n) =
 *            Σ_{k=1}^{min(i, j+1)} (i - k + 1)^2 · (j + 2 - k)^2 For i = 0
 *            the value is 1
 * Range:     u8 n <= 12, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t row_index(idx_t n) {
    /* binary search for i such that i*(i+1)/2 <= n < (i+1)*(i+2)/2 */
    idx_t lo = 0, hi = 1;
    while (((unsigned __int128)hi * (hi + 1)) / 2 <= n) {
        hi <<= 1;
    }
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        if (((unsigned __int128)mid * (mid + 1)) / 2 <= n) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    return lo;
}

seq_t a(idx_t n) {
    idx_t i = row_index(n);
    if (i == 0) {
        return (seq_t)1;
    }
    idx_t j = n - (i * (i + 1)) / 2;
    idx_t limit = (j + 1 < i) ? (j + 1) : i; /* min(i, j+1) */
    seq_t sum = 0;
    for (idx_t k = 1; k <= limit; ++k) {
        seq_t x1 = (seq_t)(i - k + 1);
        x1 *= x1;                     /* (i - k + 1)^2 */
        seq_t x2 = (seq_t)(j + 2 - k);
        x2 *= x2;                     /* (j + 2 - k)^2 */
        sum += x1 * x2;
    }
    return sum;
}
