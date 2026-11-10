/*
 * A074642  Numerator of A074639(n)/A000010(n).
 * https://oeis.org/A074642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of A074639(n)/A000010(n), where A074639(n) =
 *            Sum_{h in [1,n], gcd(h,n)=1} (h*h' - 1)/n with h*h' == 1 (mod
 *            n), and A000010 is Euler's totient
 * Range:     u8 n <= 16, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A074642_inv(idx_t a, idx_t m)
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

static idx_t A074642_gcd(idx_t a, idx_t b)
{
    while (b != 0) { idx_t t = a % b; a = b; b = t; }
    return a;
}

static seq_t A074642_gcds(seq_t a, seq_t b)
{
    while (b != 0) { seq_t t = a % b; a = b; b = t; }
    return a;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;

    seq_t sum = (seq_t)0;
    for (idx_t h = 1; h < n; ++h) {
        if (A074642_gcd(h, n) != 1) continue;
        idx_t inv = A074642_inv(h, n);
        seq_t prod = (seq_t)h * (seq_t)inv;
        sum += (prod - (seq_t)1) / (seq_t)n;
    }

    /* totient */
    seq_t phi = 1;
    idx_t m = n;
    for (idx_t p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            m /= p;
            phi *= (seq_t)(p - 1);
            while (m % p == 0) {
                m /= p;
                phi *= (seq_t)p;
            }
        }
    }
    if (m > 1) phi *= (seq_t)(m - 1);

    if (sum == 0) return (seq_t)0;
    seq_t g = A074642_gcds(sum, phi);
    return sum / g;
}
