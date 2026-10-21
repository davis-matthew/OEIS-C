/*
 * A215247  A Beatty sequence: a(n) = floor((n-1/2)*(2 + 2*sqrt(2))).
 * https://oeis.org/A215247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n-1/2)*(2 + 2*sqrt(2))) =
 *            floor((2n-1)*(1+sqrt(2)))
 * Range:     u8 n <= 53, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

/* integer square root for unsigned long long (floor(sqrt(x))) */
static inline uint64_t isqrt_ull(uint64_t x)
{
    uint64_t lo = 0, hi = ULLONG_MAX >> 1;      /* sqrt(ULLONG_MAX) <= 2^32-1 */
    while (lo <= hi) {
        uint64_t mid = (lo + hi) >> 1;
        uint64_t sq  = mid * mid;
        if (sq == x)
            return mid;
        if (sq < x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

/* Core sequence function */
seq_t a(idx_t n)
{
    /* n is 1‑based; sequence defined for n >= 1 */
    /* k = 2n-1 */
    idx_t k = (idx_t)2 * n - (idx_t)1;

    /* Use a wider type for intermediate computations */
    uint64_t kk = (uint64_t)k;
    uint64_t val = 2ULL * kk * kk;           /* val = 2*k*k  */

    uint64_t s = isqrt_ull(val);             /* s = floor(sqrt(2*k*k)) = floor(k*sqrt(2)) */

    /* a(n) = k + s ; cast to seq_t (overflow with wrap‑around on small types) */
    return (seq_t)(k + s);
}
