/*
 * A184366  G.f.: eta(x)^3*(1 - x*eta'(x)/eta(x)), where eta(x) is Dedekind's
 *          eta(q) function without the q^(1/24) factor.
 * https://oeis.org/A184366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -(−1)^k * (k-2)(k+3)(2k+1)/6 if n = k(k+1)/2 for some k ≥
 *            0. Otherwise a(n) = 0. The computation checks whether n is
 *            triangular by solving k(k+1)/2 = n. It uses an integer
 *            square‑root routine that is O(log n) and works for all 64‑bit
 *            indices
 * Range:     i8 n <= 35, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root: floor(sqrt(n)) for 64‑bit unsigned n. */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = n;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* Check whether n is a triangular number. */
    uint64_t disc   = 1 + 8 * n;           /* 1 + 8n */
    uint64_t t      = isqrt_u64(disc);     /* floor(sqrt(1+8n)) */
    uint64_t k      = (t - 1) >> 1;        /* candidate integer k */

    __uint128_t tri = (__uint128_t)k * (k + 1) / 2;
    if ((uint64_t)tri != n)                /* not triangular */
        return 0;

    /* Compute the coefficient: -(−1)^k *(k-2)(k+3)(2k+1)/6 */
    __int128 k128  = ( __int128)k;
    __int128 prod  = (k128 - 2) * (k128 + 3) * (2 * k128 + 1);
    __int128 val   = prod / 6;             /* integer division */

    if ((k & 1) == 0)                      /* k even → negate */
        val = -val;

    return (seq_t)val;
}
