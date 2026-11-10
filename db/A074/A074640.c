/*
 * A074640  a(n) = A074639(n) mod A000010(n).
 * https://oeis.org/A074640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A074639(n) mod A000010(n), where A074639(n) = Sum_{h in
 *            [1,n], gcd(h,n)=1} (h*h'-1)/n with h*h' == 1 (mod n); the sum is
 *            computed directly and reduced mod phi(n) as it goes
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A000010_phi(idx_t n)
{
    idx_t result = n;
    idx_t m = n;
    idx_t p;
    for (p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            result -= result / p;
        }
    }
    if (m > 1) result -= result / m;
    return result;
}

static idx_t A074640_inv(idx_t a, idx_t m)
{
    idx_t r0 = a, r1 = m;
    int64_t s0 = 1, s1 = 0;
    while (r1 != 0) {
        idx_t q = r0 / r1;
        idx_t r2 = r0 % r1;
        r0 = r1; r1 = r2;
        int64_t s2 = s0 - (int64_t)q * s1;
        s0 = s1; s1 = s2;
    }
    int64_t inv = s0 % (int64_t)m;
    if (inv < 0) inv += (int64_t)m;
    return (idx_t)inv;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;
    idx_t phi = A000010_phi(n);
    idx_t sum = 0;
    for (idx_t h = 1; h < n; ++h) {
        idx_t x = h, y = n;
        while (y != 0) { idx_t t = x % y; x = y; y = t; }
        if (x != 1) continue;
        idx_t inv = A074640_inv(h, n);
        idx_t k = (h * inv - 1) / n;
        sum = (sum + k % phi) % phi;
    }
    return (seq_t)sum;
}
