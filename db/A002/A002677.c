/*
 * A002677  Denominators of coefficients for central differences
 *          M_{3}'^(2*n+1).
 * https://oeis.org/A002677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of 2*(4^n-1)/(2n+1)!, i.e. a(n) = (2n+1)! /
 *            gcd(2*(4^n-1), (2n+1)!). The coefficient of x^n in (1/2) d/dx (2
 *            sinh(sqrt(x)/2))^4 = (1/2) d/dx (2cosh(2y) - 8cosh(y) + 6), y =
 *            sqrt(x), is 2*(4^n-1)/(2n+1)!; the code computes the p-adic
 *            valuations of this fraction prime by prime
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   a002677_pm
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a002677_pm.h"

seq_t a(idx_t n)
{
    uint64_t N = 2 * (uint64_t)n + 1;
    unsigned char *comp = (unsigned char *)malloc((size_t)N + 1);
    uint64_t i, j;
    seq_t res = 1;
    uint64_t chunk = 1;
    memset(comp, 0, (size_t)N + 1);
    for (i = 2; i * i <= N; i++)
        if (!comp[i])
            for (j = i * i; j <= N; j += i) comp[j] = 1;
    for (i = 2; i <= N; i++) {
        uint64_t p = i, vf = 0, v = 0, e, q, k;
        if (comp[i]) continue;
        for (q = N; q; ) { q /= p; vf += q; }
        if (p == 2) {
            v = 1;
        } else {
            uint64_t m = n, s = 0;
            while (m % p == 0) { m /= p; s++; }
            if (a002677_pm(4, m, p) == 1) {
                uint64_t pk = p;
                v = 1;
                while (v < vf && pk <= UINT64_MAX / p) {
                    uint64_t pk2 = pk * p;
                    if (a002677_pm(4, m, pk2) == 1) { v++; pk = pk2; }
                    else break;
                }
                v += s;
            }
        }
        e = vf > v ? vf - v : 0;
        for (k = 0; k < e; k++) {
            if (chunk > UINT64_MAX / p) {
                res *= (seq_t)chunk;
                chunk = 1;
            }
            chunk *= p;
        }
    }
    res *= (seq_t)chunk;
    free(comp);
    return res;
}
