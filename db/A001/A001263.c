/*
 * A001263  Triangle of Narayana numbers T(n,k) = C(n-1,k-1)*C(n,k-1)/k with 1
 *          <= k <= n, read by rows. Also called the Catalan triangle.
 * https://oeis.org/A001263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl, nice, look, changed
 * Formula:   a(n) = T(r,k) = C(r-1,k-1)*C(r,k-1)/k, where row r is the least
 *            r with r*(r+1)/2 >= n and k = n - r*(r-1)/2
 * Range:     u8 n <= 31, u16 n <= 82, u32 n <= 219, u64 n <= 720,
 *            u128 n <= 2517, big any n
 * Validated: standard, pass, 82 terms through n = 4985 (2026-11-01)
 * Helpers:   binomial, gcd, isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/gcd.h"
#include "../utils/isqrt_u64.h"

seq_t a(idx_t n) {
    uint64_t sq = isqrt_u64(1 + 8 * (uint64_t)n);
    idx_t r = (sq - 1) / 2;
    idx_t k;
    seq_t b1, b2, kk, g;
    if (r < 1) r = 1;
    while (r > 1 && (r - 1) * r / 2 >= n) r--;
    while (r * (r + 1) / 2 < n) r++;
    k = n - r * (r - 1) / 2;

    b1 = binomial(r - 1, k - 1);
    b2 = binomial(r, k - 1);
    kk = (seq_t)k;
    g = gcd(b1, kk);
    b1 = b1 / g;
    kk = kk / g;
    b2 = b2 / kk;
    return b1 * b2;
}
