/*
 * A226733  G.f.: 1 / (1 + 8*x*G(x)^2 - 10*x*G(x)^3) where G(x) = 1 + x*G(x)^4
 *          is the g.f. of A002293.
 * https://oeis.org/A226733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} C(2*k, n-k) * C(4*n-2*k, k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the provided binomial routine available in
   the utils library. */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        /* C(2*k, n-k) */
        seq_t term1 = binomial(2 * k, n - k);
        /* C(4*n-2*k, k) */
        seq_t term2 = binomial(4 * n - 2 * k, k);
        sum += term1 * term2;
    }
    return sum;
}
