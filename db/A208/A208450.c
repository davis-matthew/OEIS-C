/*
 * A208450  Denominator of A010786(n+1) / A010786(n).
 * https://oeis.org/A208450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = A010786(n) / gcd(A010786(n), A010786(n+1)), A010786(n) =
 *            Product_{k=1..n} floor(n/k); computed via prime exponents
 * Range:     u8 n <= 14, u16 n <= 38, u32 n <= 94, u64 n <= 358,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 69 terms through n = 997 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static void A208450_add(long long *d, uint64_t m, long long sign)
{
    uint64_t k = 1;
    while (k <= m) {
        uint64_t v = m / k;
        uint64_t nk = m / v + 1;
        long long cnt = (long long)(nk - k);
        uint64_t x = v;
        for (uint64_t p = 2; p * p <= x; p++) {
            if (x % p == 0) {
                long long e = 0;
                while (x % p == 0) { x /= p; e++; }
                d[p] += sign * e * cnt;
            }
        }
        if (x > 1) d[x] += sign * cnt;
        k = nk;
    }
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    long long *d = (long long *)calloc((size_t)(N + 2), sizeof(long long));
    A208450_add(d, N, 1);
    A208450_add(d, N + 1, -1);
    seq_t res = (seq_t)1;
    for (uint64_t p = 2; p <= N + 1; p++) {
        for (long long e = d[p]; e > 0; e--)
            res *= (seq_t)p;
    }
    free(d);
    return res;
}
