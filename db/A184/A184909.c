/*
 * A184909  a(n) = n + floor(n*s/r) + floor(n*t/r), where r=2^(1/5), s=r^2,
 *          t=r^3.
 * https://oeis.org/A184909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*r) + floor(n*r^2) with r = 2^(1/5)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   power_leq_limit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/power_leq_limit.h"

/* Integer 5th root: floor(cuberoot_5(limit)) */
static seq_t fifth_root(seq_t limit)
{
    seq_t lo = 0, hi = limit;
    while (lo < hi) {
        seq_t mid = lo + (hi - lo + 1) / 2; /* ceil mid to avoid infinite loop */
        if (power_leq_limit(mid, limit))
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;

    /* Compute n^5 */
    seq_t n5 = 1;
    for (unsigned int i = 0; i < 5; ++i) n5 *= N;

    /* Compute floor(n*r) = floor((2*n^5)^(1/5)) */
    seq_t val1 = n5 * 2;
    seq_t r1 = fifth_root(val1);

    /* Compute floor(n*r^2) = floor((4*n^5)^(1/5)) */
    seq_t val2 = n5 * 4;
    seq_t r2 = fifth_root(val2);

    return N + r1 + r2;
}
