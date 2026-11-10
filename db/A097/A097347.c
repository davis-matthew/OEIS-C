/*
 * A097347  Rectangular array, by antidiagonals: T(n,k) = rank of k-th n in
 *          A097291.
 * https://oeis.org/A097347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   antidiagonal d, i-th entry is T(r,c) with r=i, c=d+1-i;
 *            T(1,c)=(c-1)^2+1; for r>=2: T(r,1)=(r-1)^2+2, T(r,2)=(r-1)^2+3,
 *            T(r,c)=(r-1)^2+2c-1 for 3<=c<=r, T(r,c)=(c-1)^2+2r for c>r
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 71 terms through n = 71 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A097347_isqrt(idx_t n)
{
    idx_t lo = 0, hi = 4294967296ULL;
    while (lo + 1 < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (mid <= n / (mid ? mid : 1))
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* find d with d(d-1)/2 < n <= d(d+1)/2 */
    idx_t d = A097347_isqrt(2 * n);
    if (d < 1) d = 1;
    while (d * (d + 1) / 2 < n) ++d;
    while (d > 1 && (d - 1) * d / 2 >= n) --d;
    idx_t i = n - d * (d - 1) / 2;
    idx_t r = i, c = d + 1 - i;

    seq_t R = (seq_t)r, C = (seq_t)c;
    if (r == 1)
        return (C - 1) * (C - 1) + 1;
    if (c > r)
        return (C - 1) * (C - 1) + 2 * R;
    seq_t base = (R - 1) * (R - 1);
    if (c == 1) return base + 2;
    if (c == 2) return base + 3;
    return base + 2 * C - 1;
}
