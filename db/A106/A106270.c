/*
 * A106270  Inverse of number triangle A106268; triangle T(n,k), 0 <= k <= n.
 * https://oeis.org/A106270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) is the (row‑major) entry of the number triangle T(r,k) = 1
 *            if k == r -Catalan(r-k) otherwise. For n = r*(r+1)/2 + k with 0
 *            ≤ k ≤ r a(n) = (k == r) ? 1 : -Catalan(r-k)
 * Range:     i8 n <= 14, i16 n <= 44, i32 n <= 209, i64 n <= 665,
 *            i128 n <= 1325, big any n
 * Validated: standard, pass, 82 terms through n = 1257 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <sys/types.h>
#include "../utils/binomial.h"

/* Computes the integer r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2.
   Binary search on the range [0, n] using 128‑bit arithmetic
   to avoid overflow */
static idx_t triangular_row(idx_t n)
{
    idx_t lo = 0, hi = n, ans = 0;

    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        __int128 sum = (__int128)mid * (mid + 1) / 2;
        if (sum <= n) {
            ans = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* find row r and column k */
    idx_t r = triangular_row(n);
    idx_t k = n - (idx_t)((__int128)r * (r + 1) / 2);
    /* if last column, value is 1 */
    if (k == r) return (seq_t)1;
    /* compute -Catalan(r-k) */
    idx_t m = r - k;                     /* 0 < m ≤ r */
    seq_t catalan = binomial(2 * m, m);     /* C(2m, m) */
    catalan /= (seq_t)(m + 1);           /* divide by (m+1) */
    return -catalan;
}
