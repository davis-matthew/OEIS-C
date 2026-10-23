/*
 * A181511  Triangle T(n,k) = n!/(n-k)! read by rows, 0 <= k < n.
 * https://oeis.org/A181511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = r!/(r-k)! where r is the row containing n (so that
 *            r(r-1)/2 < n ≤ r(r+1)/2) and k = n - r(r-1)/2 - 1. This is the
 *            flattened row‑by‑row sequence of the triangle T(n,k) = n!/(n-k)!
 * Range:     u8 n <= 19, u16 n <= 43, u32 n <= 90, u64 n <= 229,
 *            u128 n <= 626, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Find the row r such that r(r-1)/2 < n ≤ r(r+1)/2. */
    idx_t low = 1;
    idx_t high = n;               /* n is always ≥1, so r ≤ n.   */
    while (low < high) {
        idx_t mid = (low + high) >> 1;            /* (low+high)/2, avoid overflow  */
        idx_t sum = mid * (mid + 1U) / 2U;        /* mid*(mid+1)/2 fits in idx_t   */
        if (sum >= n)
            high = mid;
        else
            low = mid + 1U;
    }
    idx_t r = low;

    /* Compute the offset k within row r.  */
    idx_t prev_sum = (r - 1U) * r / 2U;           /* (r-1)*r/2  */
    idx_t k = n - prev_sum - 1U;                  /* 0‑based column index */

    /* Compute the falling factorial r*(r-1)*...*(r-k+1). */
    seq_t val = 1U;
    for (idx_t t = 0U; t < k; ++t)
        val *= (seq_t)(r - t);

    return val;
}
