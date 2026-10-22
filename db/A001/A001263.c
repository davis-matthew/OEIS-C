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
 * Helpers:   a000037_isqrt, a001263_binom, a001263_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a000037_isqrt.h"
#include "../utils/a001263_binom.h"
#include "../utils/a001263_gcd.h"

seq_t a(idx_t n) {
    uint64_t sq = a000037_isqrt(1 + 8 * (uint64_t)n);
    idx_t r = (sq - 1) / 2;
    idx_t k;
    seq_t b1, b2, kk, g;
    if (r < 1) r = 1;
    while (r > 1 && (r - 1) * r / 2 >= n) r--;
    while (r * (r + 1) / 2 < n) r++;
    k = n - r * (r - 1) / 2;

    b1 = a001263_binom(r - 1, k - 1);
    b2 = a001263_binom(r, k - 1);
    kk = (seq_t)k;
    g = a001263_gcd(b1, kk);
    b1 = b1 / g;
    kk = kk / g;
    b2 = b2 / kk;
    return b1 * b2;
}
