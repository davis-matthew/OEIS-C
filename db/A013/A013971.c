/*
 * A013971  a(n) = sigma_23(n), the sum of the 23rd powers of the divisors of
 *          n.
 * https://oeis.org/A013971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = sigma_23(n) = sum_{d|n} d^23
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 6, u128 n <= 47,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 1)           /* a(1) is the first valid term */
        return 0;

    uint64_t nn = (uint64_t)n;

    /* Arrays for prime factors and their exponents */
    /* 64 is ample for any 64‑bit integer */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;

    /* factor_u64 populates primes[i] and exps[i] for i=0..cnt-1 */
    factor_u64(nn, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* Compute p^23 once (may overflow 64‑bit if it doesn't fit,
           but that's acceptable – the result will overflow as well). */
        seq_t p_pow23 = big_pow((seq_t)p, (seq_t)23);

        /* Compute 1 + p^23 + (p^23)^2 + … + (p^23)^e  */
        seq_t term = 1;
        seq_t cur  = 1;
        for (uint64_t j = 1; j <= e; ++j) {
            cur  = cur * p_pow23;
            term = term + cur;
        }

        result = result * term;
    }

    return result;
}
