/*
 * A039696  If n = Product (p_j^k_j) then a(n) = Product (p_j) + Product
 *          (k_j).
 * https://oeis.org/A039696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product of distinct prime factors + product of their
 *            exponents
 * Range:     u8 n <= 254, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Outside the offset (n == 0) is unused; return 0. */
    if (n == 0)
        return (seq_t)0;

    /* Factorize n.  For n == 1 the factor list is empty, so cnt == 0. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t prod_primes = (seq_t)1;
    seq_t prod_exps   = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        prod_primes *= (seq_t)primes[i];
        prod_exps   *= (seq_t)exps[i];
    }

    return prod_primes + prod_exps;
}
