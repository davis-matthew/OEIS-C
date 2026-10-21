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
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

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
