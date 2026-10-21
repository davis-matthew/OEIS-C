/*
 * A060144  a(n) = floor(n/(1+phi)), or equivalently floor(n/phi^2), where phi
 *          is the golden ratio (A001622).
 * https://oeis.org/A060144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n/(1+phi)) = floor(n/phi^2)
 * Range:     u8 n <= 670, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(val)) for val <= 2^128-1 */
static inline uint64_t isqrt_u128(unsigned __int128 val)
{
    uint64_t lo = 0;
    uint64_t hi;
    if (val >> 64) {               /* val exceeds 64‑bit range               */
        hi = UINT64_MAX;           /* upper bound large enough to contain sqrt */
    } else {
        uint64_t v = (uint64_t)val;
        hi = (uint64_t)((v * 3ULL) + 1ULL); /* sqrt(5)*n < 3*n  */
        if (hi == 0) hi = 1;       /* avoid zero when n = 0      */
    }
    uint64_t best = 0;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * (unsigned __int128)mid;
        if (sq <= val) {
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
    /* n is 0‑based */
    uint64_t n64 = n;
    if (n64 == 0) return 0;

    /* val = 5*n^2 */
    unsigned __int128 val = (unsigned __int128)5 * n64 * n64;

    /* floor(sqrt(val)) */
    uint64_t s_floor = isqrt_u128(val);

    /* ceil(sqrt(val)) */
    uint64_t s_ceil = s_floor;
    if ((unsigned __int128)s_floor * s_floor < val) {
        s_ceil = s_floor + 1;
    }

    /* numerator = 3*n - ceil_sqrt */
    unsigned __int128 num = (unsigned __int128)3 * n64 - s_ceil;

    /* a(n) = floor(num / 2) */
    uint64_t res = (uint64_t)(num >> 1);   /* division by 2 (floor) */

    return (seq_t)res;
}
