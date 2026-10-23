/*
 * A182766  Beatty sequence for 5 - 2*sqrt(2).
 * https://oeis.org/A182766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((5-2*sqrt(2)) * n)
 * Range:     u8 n <= 117, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root for 64‑bit values: floor( sqrt(x) ) */
static inline uint64_t isqrt64(uint64_t x)
{
    uint64_t lo = 0, hi = (x < 0xffffffffULL) ? x : 0xffffffffULL;
    uint64_t mid, best = 0;
    while (lo <= hi) {
        mid = (lo + hi) >> 1;
        uint64_t sq = mid * mid;
        if (sq == x) return mid;
        if (sq < x) {
            best = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return best;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* behaviour below offset is irrelevant */

    /* compute floor(2*n*sqrt(2)) as floor( sqrt(2*(2n)^2) ) */
    uint64_t m   = 2 * (uint64_t)n;                 /* 2n  */
    uint64_t val = m * m;                          /* (2n)^2  */
    val = val * 2ULL;                              /* 2*(2n)^2  */

    uint64_t floor2nsqrt2 = isqrt64(val);

    /*  a(n) = 5*n - 1 - floor(2*n*sqrt(2))   */
    uint64_t result = 5 * (uint64_t)n - 1 - floor2nsqrt2;

    return (seq_t)result;
}
