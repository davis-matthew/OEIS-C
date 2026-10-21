/*
 * A073890  Numerator of n/floor(sqrt(n)).
 * https://oeis.org/A073890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, look
 * Formula:   a(n) = n / gcd(n, floor(sqrt(n))) (numerator of n /
 *            floor(sqrt(n)))
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


/* Integer square‑root: floor(sqrt(v)) for 0 <= v < 2^64 */
static uint64_t isqrt_u64(uint64_t v)
{
    uint64_t r = 0;
    for (int i = 63; i >= 0; --i) {
        uint64_t bit = 1ULL << i;
        uint64_t t   = r | bit;
        if (t <= v / t)   /* avoid overflow: t*t <= v  <=>  v/t >= t  (t>0) */
            r = t;
    }
    return r;
}

/* Euclidean GCD for 64‑bit unsigned integers */
static uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0)                 /* offset starts at 1, guard for safety */
        return 0;

    uint64_t num = (uint64_t)n;
    uint64_t den = isqrt_u64(num);
    uint64_t g   = gcd_u64(num, den);

    return (seq_t)(num / g);
}
