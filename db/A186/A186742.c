/*
 * A186742  Expansion of f(x, x^11) in powers of x where f(, ) is Ramanujan's
 *          general theta function.
 * https://oeis.org/A186742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if there exists k∈ℤ with n = 6k² − 5k, otherwise 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root for 64‑bit unsigned values.
   Uses binary search with overflow‑free comparison. */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = (idx_t)1U << 32;          /* √(2⁶⁴−1) < 2³² */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid > x / mid)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* avoid overflow in 24*n (UINT64_MAX ≈ 1.8447×10¹⁹) */
    if (n > ((UINT64_MAX - 25ULL) / 24ULL))
        return 0;   /* Coefficient is 0 for out‑of‑range n */

    idx_t s2 = 25ULL + 24ULL * n;          /* discriminant 25+24n */
    idx_t s  = isqrt_u64(s2);

    if (s * s != s2)           /* discriminant not a square -> no k */
        return 0;

    /* k = (5 ± s) / 12 is integral iff s ≡ 5 or 7 (mod 12) */
    uint8_t r = (uint8_t)(s % 12ULL);
    return (seq_t)((r == 5) || (r == 7));
}
