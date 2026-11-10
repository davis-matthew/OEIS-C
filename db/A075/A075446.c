/*
 * A075446  Numerator of A075443(n)/A000010(n).
 * https://oeis.org/A075446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of A075443(n)/phi(n) =
 *            A075443(n)/gcd(A075443(n), phi(n)), where A075443(n) =
 *            (1/2)*Sum_{1<=h<n, gcd(h,n)=1} |h - h^-1 mod n| and phi =
 *            A000010
 * Range:     u8 n <= 30, u16 n <= 640, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9492 (2026-11-07)
 * Helpers:   gcd_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_idx.h"

static idx_t A075446_mod_inverse(idx_t a, idx_t mod)
{
    int64_t t = 0;
    int64_t newt = 1;
    int64_t r = (int64_t)mod;
    int64_t newr = (int64_t)a;

    while (newr != 0) {
        int64_t q = r / newr;
        int64_t tmp;

        tmp = t - q * newt;
        t = newt;
        newt = tmp;

        tmp = r - q * newr;
        r = newr;
        newr = tmp;
    }
    if (t < 0) t += (int64_t)mod;
    return (idx_t)t;
}

static seq_t A075446_A075443(idx_t n)
{
    seq_t sum = 0;
    for (idx_t h = 1; h < n; ++h) {
        if (gcd_idx(h, n) != 1) continue;
        idx_t inv = A075446_mod_inverse(h, n);
        idx_t diff = (h > inv) ? h - inv : inv - h;
        sum += (seq_t)diff;
    }
    return sum / 2;
}

static seq_t A075446_A000010(idx_t n)
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

static seq_t A075446_gcd(seq_t x, seq_t y)
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
    seq_t s = A075446_A075443(n);
    seq_t ph = A075446_A000010(n);
    seq_t g = A075446_gcd(s, ph);
    if (g == 0) return 0;
    return s / g;
}
