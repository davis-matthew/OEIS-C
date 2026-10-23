/*
 * A144480  T(n,k) = binomial(n, k)*min(k + 1, n - k + 1), triangle read by
 *          rows (n >= 0, 0 <= k <= n).
 * https://oeis.org/A144480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = binomial(r,k) * min(k+1, r-k+1) where r = ⌊(√(8n+1)-1)/2⌋
 *            is the row number and k = n - r(r+1)/2 is the column number in
 *            that row
 * Range:     u8 n <= 39, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

static idx_t find_row(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        __uint128_t val = (__uint128_t)mid * (mid + 1) >> 1;
        if (val <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    idx_t r = find_row(n);
    __uint128_t row_start_u128 = (__uint128_t)r * (r + 1) >> 1;
    idx_t row_start = (idx_t)row_start_u128;
    idx_t k = n - row_start;
    seq_t bin = binomial(r, k);
    idx_t m = (k + 1 <= r - k + 1) ? (k + 1) : (r - k + 1);
    return bin * (seq_t)m;
}
