/*
 * A187535  Central Lah numbers: a(n) = A105278(2*n,n) = A008297(2*n,n).
 * https://oeis.org/A187535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n = 0 * a(n) = binomial(2*n-1, n-1) * (2*n)! / n!
 *            for n > 0 * * = binomial(2*n-1, n-1) * ∏_{k=n+1}^{2*n} k * *
 *            This implementation uses the library routine `binomial` from *
 *            ../utils/binomial.h and performs the product iteratively
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    if (n == 0u)
        return (seq_t)1;

    /* binomial(2n-1, n-1) */
    seq_t result = binomial(2 * n - 1, n - 1);

    /* multiply by (n+1)(n+2)...(2n) = (2n)! / n! */
    for (idx_t k = n + 1; k <= 2 * n; ++k)
    {
        result *= (seq_t)k;
    }

    return result;
}
