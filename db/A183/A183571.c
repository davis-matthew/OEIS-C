/*
 * A183571  n+floor(sqrt(n+2)).
 * https://oeis.org/A183571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(sqrt(n+2))
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root for unsigned 64‑bit values, floor(sqrt(x)) */
static inline idx_t isqrt(idx_t x)
{
    idx_t lo = 0;
    idx_t hi = 1ULL << 32;          /* exclusive upper bound; (2^32)^2 = 2^64 */
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        /* mid*mid fits in idx_t because mid <= 2^32-1 */
        idx_t sq = mid * mid;
        if (sq <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* Compute floor(sqrt(n + 2)) */
    idx_t s = isqrt(n + 2);
    seq_t result = (seq_t)n;
    result += (seq_t)s;
    return result;
}
