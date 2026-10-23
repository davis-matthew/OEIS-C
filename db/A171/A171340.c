/*
 * A171340  Number of 1..n integer arrays v[1..3] of length 3 with all
 *          autocorrelation values sum(i){v[i]*v[i-k]} distinct for k in 0..2.
 * https://oeis.org/A171340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^3 - #{(a,b,c) | 1≤a,b,c≤n, a*b + b*c = a*c}
 * Range:     u8 n <= 6, u16 n <= 32, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* Euclidean GCD for unsigned integers */
static inline idx_t _gcd(idx_t x, idx_t y)
{
    while (y) {
        idx_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

/* integer floor of sqrt(x) for idx_t */
static inline idx_t _floor_sqrt(idx_t x)
{
    if (x == 0ULL || x == 1ULL) return x;
    idx_t r = (idx_t)floor(sqrt((double)x));
    while ((r + 1ULL) * (r + 1ULL) <= x) ++r;
    while (r * r > x) --r;
    return r;
}

/* target function */
seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t total = N * N * N;   /* n^3 */

    idx_t limit = _floor_sqrt(n);
    seq_t violations = 0;

    for (idx_t ap = 1; ap <= limit; ++ap) {
        for (idx_t cp = 1; cp <= limit; ++cp) {
            if (_gcd(ap, cp) != 1) continue;

            idx_t denom1 = ap * (ap + cp);
            idx_t denom2 = cp * (ap + cp);

            /* if denominators exceed n, no contribution */
            if (denom1 > n || denom2 > n) continue;

            idx_t k1 = n / denom1;
            idx_t k2 = n / denom2;
            idx_t kmax = k1 < k2 ? k1 : k2;
            if (kmax > 0) violations += (seq_t)kmax;
        }
    }

    return total - violations;
}
