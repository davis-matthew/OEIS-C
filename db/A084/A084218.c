/*
 * A084218  a(n) = sigma_4(n^2)/sigma_2(n^2).
 * https://oeis.org/A084218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = sigma_4(n^2)/sigma_2(n^2) Multiplicative definition:
 *            a(p^e) = (p^(4*e+2)+1)/(p^2+1) for prime powers p^e
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* outside the sequence offset */

    /* Factorisation helpers */
    uint64_t primes[64];
    uint64_t exps [64];
    size_t   cnt    = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p  = (seq_t)primes[i];   /* prime */
        uint64_t e = exps[i];          /* exponent */

        seq_t p2 = p * p;          /* p^2   */
        seq_t p4 = p2 * p2;        /* p^4   */

        seq_t factor = 1;
        for (uint64_t j = 0; j < e; ++j) {
            /* factor_{j+1} = factor_j * p^4 - p^2 + 1   */
            factor = factor * p4 - p2 + 1;
        }

        result = result * factor;
    }

    return result;
}
