/*
 * A028275  Elements in 4-Pascal triangle (by row).
 * https://oeis.org/A028275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is obtained by flattening the 4‑Pascal triangle, whose
 *            entry T(r,k) satisfies T(r,k) = C(r,k) + 2*C(r-2,k-1) (with the
 *            second binomial treated as 0 when indices are invalid)
 * Range:     u8 n <= 39, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    if (n > 0) {
        idx_t lo = 0, hi = n;
        while (lo <= hi) {
            idx_t mid = (lo + hi) >> 1;
            idx_t start = mid * (mid + 1) / 2;
            if (start <= n) {
                r = mid;
                lo = mid + 1;
            } else {
                if (mid == 0) break;
                hi = mid - 1;
            }
        }
    }

    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;

    seq_t res = binomial(r, k);
    if (r >= 2 && k >= 1 && k - 1 <= r - 2) {
        seq_t add = binomial(r - 2, k - 1);
        res += (seq_t)2 * add;
    }
    return res;
}
