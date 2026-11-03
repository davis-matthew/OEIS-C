/*
 * A184114  n + floor(5*sqrt(n-1)); complement of A184115.
 * https://oeis.org/A184114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(5*sqrt(n-1))
 * Range:     u8 n <= 64, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* Handle the (undefined) offset case, though it is never
       queried by the tests. */
    if (n == 0)
        return 0;

    /* floor(sqrt(n-1)) */
    seq_t s = floor_sqrt((seq_t)(n - 1));

    /* 25 * (n-1) as an unsigned 128‑bit integer to avoid overflow. */
    unsigned __int128 limit = (unsigned __int128)(n - 1) * 25ULL;

    /* Candidate for floor(5*sqrt(n-1)) */
    unsigned long long t = (unsigned long long)s * 5ULL;

    /* Refine the candidate: the true value is the largest t
       with t^2 <= limit.  At most 5 iterations are required. */
    while ((unsigned __int128)t * t <= limit)
        t++;
    t--;   /* step back to the floor */

    return (seq_t)n + (seq_t)t;
}
