/*
 * A211512  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          w^2=3x*y.
 * https://oeis.org/A211512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered triples (w,x,y) with 1 ≤ w,x,y ≤ n and
 *            w² = 3·x·y. For each w divisible by 3 (w = 3k), we let m = 3k².
 *            The number of ordered pairs (x,y) with xy = m and 1 ≤ x,y ≤ n is
 *            counted by iterating over all divisors d of m up to √m. If d·e =
 *            m with d ≤ e, and d ≤ n and e ≤ n, we a...
 * Range:     u8 n <= 14, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 66 (2026-11-01)
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
    seq_t total = 0;
    idx_t maxK = n / 3;
    seq_t limit = (seq_t)n;

    for (idx_t kk = 1; kk <= maxK; ++kk)
    {
        seq_t k = (seq_t)kk;
        seq_t m = (seq_t)3 * k * k;
        seq_t sqrt_m = floor_sqrt(m);

        for (seq_t d = 1; d <= sqrt_m; ++d)
        {
            if (m % d == 0)
            {
                seq_t e = m / d;
                if (d <= limit && e <= limit)
                {
                    if (d == e)
                        ++total;
                    else
                        total += 2;
                }
            }
        }
    }
    return total;
}
