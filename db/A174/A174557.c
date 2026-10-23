/*
 * A174557  Triangle T(n, k) = -floor(n/k) with T(n, n) = 1, read by rows.
 * https://oeis.org/A174557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(r,k) where T(r,k) = -floor(r/k), T(r,r) = 1. Here r is
 *            the smallest integer with r(r+1)/2 ≥ n, and k = n - r(r-1)/2
 * Range:     i8 n <= 465, i16 n <= 465, i32 n <= 465, i64 n <= 465,
 *            i128 n <= 465, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_floor_isqrt_u64.h"

static inline __uint128_t tri128(idx_t r) {
    __uint128_t rr = r;
    return rr * (rr + 1) / 2;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;  /* undefined offset, return 0 for safety */

    /* binary search for row r: minimal r with r(r+1)/2 ≥ n */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t t = tri128(mid);
        if (t >= (__uint128_t)n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* column index k */
    __uint128_t prev = tri128(r - 1);    /* triangular(r-1), fits in idx_t */
    idx_t k = n - (idx_t)prev;

    if (k == r)
        return 1;

    seq_t val = (seq_t)(r / k);          /* floor(r/k) as signed seq_t */
    return -val;
}
