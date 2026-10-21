/*
 * A093837  Denominator of N(n)/n^2, where N(n) is the number of lattice
 *          points (x,y) with x^2 + y^2 <= n^2.
 * https://oeis.org/A093837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of N(n)/n^2, where N(n) counts integer
 *            lattice points (x,y) with x^2+y^2 ≤ n^2
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root of a 64‑bit unsigned value (pure integer). */
static inline idx_t isqrt_u64(unsigned long long v)
{
    unsigned long long low = 0;
    unsigned long long high = (v >> 1) + 1;  /* √v ≤ v/2 + 1 for v>0 */
    unsigned long long ans = 0;
    while (low <= high) {
        unsigned long long mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= v) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;  /* avoid unsigned underflow */
            high = mid - 1;
        }
    }
    return (idx_t)ans;
}

/* Euclidean GCD for 64‑bit unsigned values. */
static inline unsigned long long gcd_ull(unsigned long long a,
                                         unsigned long long b)
{
    while (b != 0) {
        unsigned long long t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) {                 /* offset is 1; return 0 for out‑of‑range */
        return (seq_t)0;
    }

    unsigned long long n2 = (unsigned long long)n * (unsigned long long)n;
    unsigned long long total = 0;

    for (unsigned long long x = 0; x <= n; ++x) {
        unsigned long long x2 = (unsigned long long)x * (unsigned long long)x;
        unsigned long long rem = n2 - x2;
        idx_t y = isqrt_u64(rem);                 /* ymax = floor(√(n²−x²)) */
        unsigned long long cnt = 2ULL * (unsigned long long)y + 1ULL; /* points for this x */

        if (x == 0) {
            total += cnt;                 /* only one x side (0) */
        } else {
            total += 2ULL * cnt;          /* two x sides (+x,−x) */
        }
    }

    unsigned long long g = gcd_ull(n2, total);
    seq_t denom = (seq_t)(n2 / g);          /* a(n) = n² / gcd(n², N(n)) */

    return denom;
}
