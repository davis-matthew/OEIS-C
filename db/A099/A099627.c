/*
 * A099627  Triangle read by rows: T(n,k) = 2^n + 2^k - 1 with n >= k >= 0.
 * https://oeis.org/A099627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   T(n,k) = 2^n + 2^k - 1, read row‑wise with n≥k≥0. This generates
 *            the sequence A099627: a(0)=1,a(1)=2,a(2)=3,a(3)=4,…
 * Range:     u8 n <= 35, u16 n <= 135, u32 n <= 527, u64 n <= 2079,
 *            u128 n <= 5150, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t lo = 0, hi = 1;

    /* Grow upper bound until it exceeds n */
    while (((unsigned __int128)hi * (hi + 1)) / 2 <= n) hi <<= 1;

    /* Binary search for the largest r with the property above */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        if (((unsigned __int128)mid * (mid + 1)) / 2 <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    idx_t r = lo;                          /* row index */
    idx_t k = n - (idx_t)(((unsigned __int128)r * (r + 1)) / 2); /* column */

    /* Compute 2^r + 2^k – 1 */
    seq_t val = pow2(r) + pow2(k) - (seq_t)1;
    return val;
}
