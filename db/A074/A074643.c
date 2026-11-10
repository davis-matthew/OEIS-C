/*
 * A074643  Denominator of A074639(n)/A000010(n).
 * https://oeis.org/A074643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of A074639(n)/phi(n), where A074639(n) =
 *            Sum_{h<=n, (h,n)=1} (h*h' - 1)/n with h*h' == 1 (mod n); a(n) =
 *            phi(n)/gcd(A074639(n), phi(n))
 * Range:     u8 n <= 16, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 71 terms through n = 71 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A074643_mod_inv(idx_t a, idx_t m)
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

static seq_t A074643_A074639(idx_t n)
{
    if (n <= 1) return (seq_t)0;
    seq_t sum = (seq_t)0;
    for (idx_t h = 1; h < n; ++h) {
        idx_t x = h, y = n;
        while (y != 0) { idx_t t = x % y; x = y; y = t; }
        if (x != 1) continue;
        idx_t inv = A074643_mod_inv(h, n);
        seq_t prod = (seq_t)h * (seq_t)inv;
        sum += (prod - (seq_t)1) / (seq_t)n;
    }
    return sum;
}

static seq_t A074643_phi(idx_t n)
{
    seq_t result = 1;
    idx_t m = n;
    idx_t p;
    for (p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            m /= p;
            result *= (seq_t)(p - 1);
            while (m % p == 0) {
                m /= p;
                result *= (seq_t)p;
            }
        }
    }
    if (m > 1)
        result *= (seq_t)(m - 1);
    return result;
}

static seq_t A074643_gcd(seq_t x, seq_t y)
{
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n)
{
    seq_t s = A074643_A074639(n);
    seq_t phi = A074643_phi(n);
    seq_t g = A074643_gcd(s, phi);
    if (g == 0) return phi;
    return phi / g;
}
