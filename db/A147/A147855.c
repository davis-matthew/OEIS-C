/*
 * A147855  G.f.: 1 / (1 + 4*x*G(x)^2 - 7*x*G(x)^3) where G(x) = 1 + x*G(x)^4
 *          is the g.f. of A002293.
 * https://oeis.org/A147855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} C(k, n-k) * C(4*n - k, k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (OEIS A147855) */

#include <stddef.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t t1 = binomial(k, n - k);          /* C(k, n-k)  */
        seq_t t2 = binomial(4 * n - k, k);      /* C(4n-k, k) */
        sum += t1 * t2;
    }
    return sum;
}
