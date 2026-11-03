/*
 * A034751  Dirichlet convolution of 3^(n-1) with itself.
 * https://oeis.org/A034751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} 3^(d-1) * 3^(n/d-1) = Sum_{d|n} 3^{d+n/d-2}
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 26 (2026-11-01)
 * Helpers:   ipow_i32, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow_i32.h"
#include "../utils/pow3.h"

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

/* Compute a(n) for n >= 1 */
seq_t a(idx_t n)
{
    if (n == 0u)      /* undefined for n==0, harmless guard */
        return 0;

    idx_t sq = (idx_t)floor_sqrt((seq_t)n);
    seq_t sum = 0;

    for (idx_t d = 1; d <= sq; ++d)
    {
        if (n % d != 0u)
            continue;

        idx_t d2 = n / d;
        /* exponent of the term: (d-1)+(d2-1) = d + d2 - 2 */
        idx_t exp = d + d2 - 2u;
        seq_t term = pow3(exp);

        sum += term;          /* add contribution for divisor d */

        if (d != d2)          /* add contribution for divisor n/d if distinct */
            sum += term;
    }

    return sum;
}
