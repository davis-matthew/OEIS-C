/*
 * A164386  Numbers which are not 1 larger than a nonzero square.
 * https://oeis.org/A164386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(sqrt(n-1) + 1/2)
 * Range:     u8 n <= 240, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer (floor) square root for 64‑bit values, no external dependency */
static inline idx_t isqrt_u64(idx_t x)
{
    /* Binary search on [0, 2^32] */
    idx_t lo = 0;
    idx_t hi = ((idx_t)1 << 32); /* >= sqrt(UINT64_MAX) */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;  /* upper mid */
        if (mid != 0 && mid > x / mid)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* defined for safety, although offset starts at 1 */

    idx_t m   = n - 1;                 /* m = n-1 */
    idx_t s   = isqrt_u64(m);          /* floor(sqrt(m)) */
    /* decide whether to round up */
    idx_t round = (m >= s * s + s + 1) ? s + 1 : s;
    return (seq_t)n + (seq_t)round;
}
