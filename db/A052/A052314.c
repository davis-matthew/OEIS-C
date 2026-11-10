/*
 * A052314  Triangle: Matrix square of A051168.
 * https://oeis.org/A052314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(r,k) = Sum_{j=k..r} L(r,j)*L(j,k), where L = A051168, * L(h,k)
 *            = (1/h) * Sum_{d | gcd(h,k)} mu(d)*C(h/d,k/d), L(0,0) = 1; *
 *            a(n) = T(r,k) with n = r*(r+1)/2 + k, 0 <= k <= r
 * Range:     u8 n <= 66, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Validated: partial, pass, 81 terms through n = 80 (2026-11-07)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

static uint64_t A052314_gcd64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static int A052314_mu(uint64_t n)
{
    if (n <= 1) return 1;
    int cnt = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            n /= p;
            if (n % p == 0) return 0;
            cnt++;
        }
    }
    if (n > 1) cnt++;
    return (cnt % 2 == 0) ? 1 : -1;
}

static seq_t A051168_L(uint64_t h, uint64_t k)
{
    if (h == 0) return (seq_t)1;
    uint64_t g = A052314_gcd64(h, k);
    seq_t pos = 0, neg = 0;
    for (uint64_t d = 1; d <= g; ++d) {
        if (g % d != 0) continue;
        int mu = A052314_mu(d);
        if (mu == 0) continue;
        seq_t b = binomial((idx_t)(h / d), (idx_t)(k / d));
        if (mu > 0) pos += b; else neg += b;
    }
    seq_t total = pos;
    total -= neg;
    return total / (seq_t)h;
}

seq_t a(idx_t n)
{
    uint64_t r = 0;
    uint64_t m = (uint64_t)n;
    while (m >= r + 1) { m -= r + 1; ++r; }
    uint64_t k = m;
    seq_t s = 0;
    for (uint64_t j = k; j <= r; ++j) {
        seq_t y = A051168_L(j, k);
        if (y == 0) continue;
        seq_t x = A051168_L(r, j);
        if (x == 0) continue;
        s += x * y;
    }
    return s;
}
