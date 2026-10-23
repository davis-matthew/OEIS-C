/*
 * A116073  Sum of the divisors of n that are not divisible by 5.
 * https://oeis.org/A116073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = sum of divisors of n not divisible by 5 (equivalently
 *            a(n) = σ(n) - 5·σ(n/5))
 * Range:     u8 n <= 16, u16 n <= 1237, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety guard (offset starts at 1) */

    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    seq_t sigma_n = 1;
    seq_t sigma_n5 = 1;
    bool has5 = false;
    size_t idx5 = (size_t)-1;

    /* compute σ(n)   */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p64 = primes[i];
        uint64_t e64 = exps[i];
        seq_t p = (seq_t)p64;

        seq_t term = seq_pow(p, (idx_t)(e64 + 1)); /* p^(e+1) */
        term -= 1;                                /* p^(e+1)-1 */
        seq_t denom = p - 1;
        term /= denom;                            /* (p^(e+1)-1)/(p-1) */
        sigma_n *= term;

        if (p64 == 5) {
            has5 = true;
            idx5 = i;
        }
    }

    if (!has5) {
        return sigma_n;
    }

    /* compute σ(n/5) by decreasing the exponent of 5 by 1 */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p64 = primes[i];
        uint64_t e64 = (i == idx5) ? (exps[i] - 1) : exps[i];
        seq_t p = (seq_t)p64;

        seq_t term = seq_pow(p, (idx_t)(e64 + 1)); /* p^(e'+1) */
        term -= 1;
        seq_t denom = p - 1;
        term /= denom;
        sigma_n5 *= term;
    }

    seq_t result = sigma_n;
    result -= sigma_n5 * (seq_t)5;
    return result;
}
