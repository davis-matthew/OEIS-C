/*
 * A167338  Totally multiplicative sequence with a(p) = p*(p+1) = p^2+p for
 *          prime p.
 * https://oeis.org/A167338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product_{p^e||n} (p*(p+1))^e
 * Range:     u8 n <= 11, u16 n <= 95, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1) {
        if (n == 0) return 0;
        return 1;
    }

    /* arrays to hold prime factors and their exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* factorise n */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t base = p * (p + 1);        /* p·(p+1)   */
        uint64_t exp = exps[i];

        /* compute base^exp */
        seq_t power = 1;
        while (exp--) {
            power *= base;
        }

        result *= power;
    }
    return result;
}
