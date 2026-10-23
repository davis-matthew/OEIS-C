/*
 * A143201  Product of distances between prime factors in factorization of n.
 * https://oeis.org/A143201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of (p_i - p_{i-1} + 1) over the ascending prime
 *            factors of n (with multiplicity). a(1) = 1
 * Range:     u8 n <= 513, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;                  /* a(1) = 1 */

    /* factor_u64 needs own arrays; 64 entries is enough for n <= 2^64 */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* If n is prime, no product of differences is needed */
    if (cnt == 1 && exps[0] == 1)
        return 1;

    seq_t result = 1;
    bool first = true;
    uint64_t prev = 0;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        for (size_t k = 0; k < exps[i]; ++k) {
            if (first) {
                first = false;
            } else {
                seq_t delta = (seq_t)(p - prev + 1);
                result *= delta;
            }
            prev = p;
        }
    }
    return result;
}
