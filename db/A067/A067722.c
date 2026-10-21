/*
 * A067722  Least positive integer k such that n*(n + k) is a perfect square.
 * https://oeis.org/A067722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   smallest positive k such that n*(n + k) is a perfect square. For
 *            n>0: a is the squarefree part of n, i.e. n = a * b^2. Then t = a
 *            * c^2 (c >= 1) must satisfy t > n. The minimal k is a * c^2 - n
 *            with c = floor( sqrt(n/a) ) + 1. For n==0, k=1
 * Range:     u8 n <= 85, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: largest r with r*r <= x  (x up to 2^64-1). */
static uint64_t isqrt_u64(uint64_t x)
{
    uint64_t lo = 0;
    uint64_t hi = (~(uint64_t)0) >> 1; /* sqrt(max_uint64) < 2^32 */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if ((__uint128_t)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Compute the sequence value. */
seq_t a(idx_t n)
{
    if (n == 0)                /* defined in OEIS as a(0) = 1 */
        return (seq_t)1;

    /* Compute squarefree part a of n. */
    uint64_t rem = (uint64_t)n;
    uint64_t a_sf = 1;
    for (uint64_t p = 2; p * p <= rem; ++p) {
        if (rem % p == 0) {
            int exp = 0;
            while (rem % p == 0) {
                rem /= p;
                exp++;
            }
            if (exp & 1)          /* odd exponent contributes to a */
                a_sf *= p;
        }
    }
    if (rem > 1)                /* remaining prime factor */
        a_sf *= rem;

    /* Compute c = floor( sqrt(n/a) ) + 1 */
    uint64_t ratio = (uint64_t)n / a_sf;
    uint64_t s = isqrt_u64(ratio);
    uint64_t c = s + 1;

    /* t = a * c^2, k = t - n */
    seq_t t = (seq_t)a_sf * (seq_t)c * (seq_t)c;
    seq_t k = t - (seq_t)n;
    return k;
}
