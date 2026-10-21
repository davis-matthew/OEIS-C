/*
 * A089204  Triangle read by rows: T(n,k) = numerator of P(n, k) = 1 -
 *          n!/(n^k*(n-k)!).
 * https://oeis.org/A089204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn, tabl
 * Formula:   a(n) = numerator of the reduced fraction P(m,k) where m(m-1)/2 <
 *            n ≤ m(m+1)/2. P(m,k) = 1 - m!/(m^k (m-k)!) for 2 ≤ k ≤ m
 * Range:     u8 n <= 5, u16 n <= 19, u32 n <= 44, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* integer square–root of a 64‑bit unsigned integer (floor). */
static idx_t isqrt_u64(idx_t n) {
    idx_t lo = 0, hi = (n >> 1) + 1;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        if ((unsigned __int128)mid * mid <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n) {
    /* locate the row m such that T(m-1) < n ≤ T(m), where T(m)=m(m-1)/2. */
    idx_t D = 1 + (idx_t)((unsigned __int128)8 * n);
    idx_t r = isqrt_u64(D);
    idx_t m = (1 + r) / 2;
    while ((unsigned __int128)m * (m - 1) / 2 < n)
        ++m;

    /* previous cumulative count of terms before row m */
    idx_t prev = (unsigned __int128)(m - 1) * (m - 2) / 2;
    idx_t offset = n - prev;      /* 1 … m‑1  */
    idx_t k = offset + 1;         /* 2 … m   */

    /* product m·(m-1)·…·(m-k+1) */
    seq_t prod = 1;
    for (idx_t i = 0; i < k; ++i)
        prod *= (seq_t)(m - i);

    /* denominator m^k */
    seq_t denom = 1;
    for (idx_t i = 0; i < k; ++i)
        denom *= (seq_t)m;

    /* reduce the fraction and take the numerator */
    seq_t g = gcd(prod, denom);
    prod  /= g;
    denom /= g;

    return denom - prod;
}
