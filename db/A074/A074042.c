/*
 * A074042  Numerator of Sum_{k=1..n} 1/A077800(k), denominator=A074043.
 * https://oeis.org/A074042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of Sum_{k=1..n} 1/A077800(k), in lowest terms;
 *            A077800 = 3,5,5,7,11,13,... (each twin prime pair {p,p+2}
 *            contributes p then p+2); computed by direct fraction summation
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 17 (2026-11-07)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static bool A074042_isprime(uint64_t x)
{
    uint64_t d;
    if (x < 2) return false;
    if (x < 4) return true;
    if (x % 2 == 0) return false;
    for (d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

static void A074042_add(seq_t *num, seq_t *den, uint64_t p)
{
    seq_t P = (seq_t)p;
    seq_t g = gcd(*den, P);
    seq_t d1 = *den / g;
    seq_t p1 = P / g;
    seq_t nn = *num * p1 + d1;
    seq_t dd = d1 * P;
    seq_t h = gcd(nn, dd);
    if (h > 1) { nn /= h; dd /= h; }
    *num = nn;
    *den = dd;
}

seq_t a(idx_t n)
{
    seq_t num = 0, den = 1;
    idx_t cnt = 0;
    uint64_t p;
    if (n < 1) return (seq_t)0;
    for (p = 3; cnt < n; p += 2) {
        if (A074042_isprime(p) && A074042_isprime(p + 2)) {
            A074042_add(&num, &den, p);
            cnt++;
            if (cnt >= n) break;
            A074042_add(&num, &den, p + 2);
            cnt++;
        }
    }
    return num;
}
