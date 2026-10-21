/*
 * A211262  Number of integer pairs (x,y) such that 0<x<y<=n and x*y=3n.
 * https://oeis.org/A211262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ (x,y) : 0 < x < y ≤ n , x*y = 3n }
 * Range:     u8 n <= 9623, u16 n <= 9623, u32 n <= 9623, u64 n <= 9623,
 *            u128 n <= 9623, big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/int_sqrt.h"

seq_t a(idx_t n)
{
    /* m = 3n, stored as idx_t (enough for all n involved) */
    idx_t m = n * 3;

    /* upper bound for factor search */
    idx_t sqrt_m = int_sqrt(m);

    seq_t count = 0;

    for (idx_t d = 1; d <= sqrt_m; ++d)
    {
        if ((m % d) != 0)
            continue;

        idx_t e = m / d;          /* complementary factor */

        /* ensure 0 < x < y ≤ n, i.e. d < e and e ≤ n */
        if (d < e && e <= n)
            ++count;
    }

    return count;
}
