/*
 * A054524  Triangle T(n,k): T(n,k) = mu(k) if k divides n, T(n,k)=0 otherwise
 *          (n >= 1, 1<=k<=n).
 * https://oeis.org/A054524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = mu(k) if k divides r, else 0, where n = r*(r-1)/2 + k, 1
 *            <= k <= r (row r, column k of the triangle)
 * Range:     i8 n <= 93, i16 n <= 93, i32 n <= 93, i64 n <= 93, i128 n <= 93,
 *            big any n
 * Validated: partial, pass, 93 terms through n = 93 (2026-11-07)
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    /* smallest r with r(r+1)/2 >= n */
    uint64_t lo = 1, hi = 6074000999ULL * 4ULL; /* r(r+1)/2 > 2^64 for r this large */
    if (hi > (uint64_t)n) hi = (uint64_t)n;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        unsigned __int128 cum = (unsigned __int128)mid * (mid + 1) / 2;
        if (cum >= (unsigned __int128)n)
            hi = mid;
        else
            lo = mid + 1;
    }
    uint64_t r = lo;
    uint64_t before = (uint64_t)((unsigned __int128)r * (r - 1) / 2);
    uint64_t k = (uint64_t)n - before;
    if (r % k != 0) return 0;
    return mobius((idx_t)k);
}
