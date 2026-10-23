/*
 * A163351  Triangle T(n,k) = m, 0<=k<=n, read by rows, where each decimal
 *          digit d of the integer m is the minimum of the corresponding
 *          decimal digits of n and k.
 * https://oeis.org/A163351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn, tabl
 * Formula:   a(n) = T(n, k) where k = n - n*(n+1)/2 and T(r, c) is the
 *            digit‑wise minimum of r and c (with left‑padded zeros)
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   min_digitwise
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/min_digitwise.h"

static idx_t floor_sqrt_u64(idx_t x)
{
    /* Integer square root using binary search with 128‑bit intermediate. */
    idx_t lo = 0, hi = (idx_t)1 << 32; /* 32‑bit guard is enough for 64‑bit x */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        __uint128_t sq = (__uint128_t)mid * (__uint128_t)mid;
        if (sq <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2. */
    idx_t temp = floor_sqrt_u64((__uint128_t)8 * n + 1);
    idx_t r = (temp - 1) / 2;

    /* Column k within that row. */
    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;

    return min_digitwise(r, k);
}
