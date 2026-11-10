/*
 * A075447  Denominator of A075443(n)/A000010(n).
 * https://oeis.org/A075447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = phi(n) / gcd(A075443(n), phi(n)), where A075443(n) =
 *            (1/2)*Sum_{1<=h<n, gcd(h,n)=1} |h - h^(-1) mod n|
 * Range:     u8 n <= 30, u16 n <= 1717, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 8933 (2026-11-07)
 * Helpers:   gcd_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_idx.h"

static idx_t A075447_inv(idx_t a, idx_t mod)
{
    int64_t t = 0, newt = 1;
    int64_t r = (int64_t)mod, newr = (int64_t)a;
    while (newr != 0) {
        int64_t q = r / newr;
        int64_t tmp = t - q * newt;
        t = newt; newt = tmp;
        tmp = r - q * newr;
        r = newr; newr = tmp;
    }
    if (t < 0) t += (int64_t)mod;
    return (idx_t)t;
}

static seq_t A075447_gcd(seq_t x, seq_t y)
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
    seq_t sum = 0;
    for (idx_t h = 1; h < n; ++h) {
        if (gcd_idx(h, n) != 1) continue;
        idx_t inv = A075447_inv(h, n);
        idx_t diff = (h > inv) ? h - inv : inv - h;
        sum += (seq_t)diff;
    }
    sum /= 2;

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

    seq_t g = A075447_gcd(phi, sum);
    return phi / g;
}
