/*
 * A105224  Number of squares between n and 2*n inclusive.
 * https://oeis.org/A105224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000196(2*n) - A000196(n-1) (= floor(sqrt(2*n)) -
 *            floor(sqrt(n-1)) )
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute floor(sqrt(x)) for an unsigned 64‑bit integer x.
   Uses binary search and 128‑bit arithmetic to avoid overflow. */
static inline idx_t isqrt_uint64(idx_t x)
{
    idx_t low = 0;
    idx_t high = (idx_t)1ULL << 32; /* sqrt(UINT64_MAX) < 2^32 */
    idx_t ans = 0;

    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t sq = (__uint128_t)mid * (__uint128_t)mid;
        if (sq <= x) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* 2 * n may overflow a 64‑bit value when n is close to 2^63;
       so cast to unsigned __int128 for the product. */
    __uint128_t two_n = (__uint128_t)n * 2;
    /* Convert two_n back to idx_t; it fits because two_n < 2^65. */
    idx_t s1 = isqrt_uint64((idx_t)two_n);
    idx_t s2 = (n > 0) ? isqrt_uint64(n - 1) : 0;
    return (seq_t)(s1 - s2);
}
