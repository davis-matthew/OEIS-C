/*
 * A093415  Triangle read by rows: a(n, k) is the denominator of (n + (n-1) +
 *          ... + (n-k+1))/(1 + 2 + ... + k), 0 < k <= n.
 * https://oeis.org/A093415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl, frac
 * Formula:   a(n) = (k+1)/gcd(2*r+2,k+1), * where r is the row number
 *            satisfying r(r+1)/2 >= n (rows start at 1), * and k = n -
 *            r(r-1)/2 is the column index within the row (1 <= k <= r)
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 >= n. */
    idx_t lo = 1, hi = 1;
    while (((unsigned __int128)hi * (hi + 1)) / 2 < n)
        hi <<= 1;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (((unsigned __int128)mid * (mid + 1)) / 2 >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;

    /* Column index k (1‑based). */
    idx_t prev_terms = ((unsigned __int128)(r - 1) * r) / 2;
    idx_t k = n - prev_terms;              /* 1 <= k <= r */

    /* Compute denominator: (k+1)/gcd(2*r+2, k+1). */
    seq_t num   = (seq_t)(k + 1);
    seq_t denom_factor = (seq_t)(2 * r + 2);
    seq_t g = gcd(num, denom_factor);
    return num / g;
}
