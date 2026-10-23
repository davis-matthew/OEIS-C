/*
 * A167515  The sum over the divisors of n, except the maximum-prime-power
 *          divisors collected in A008475.
 * https://oeis.org/A167515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000203(n) - A008475(n) a(n) = σ(n) − (sum of largest
 *            prime powers dividing n)
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses prime factorisation and elementary
   arithmetic on the provided seq_t type.                       */
#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Factorisation arrays – more than enough for n ≤ 10^6 */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* Factor n, obtaining prime bases and exponents. */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Sum of divisors σ(n).  For each prime power p^e:
        term = 1 + p + p^2 + … + p^e
        σ(n) = Π term       */
    seq_t sigma = 1;

    /* Sum of the largest prime power divisors of n. */
    seq_t sum_largest = 0;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t term = 1;          /* accumulates 1 + p + ... + p^e */
        seq_t p_pow = 1;         /* current power p^(k+1)           */

        for (size_t e = 0; e < exps[i]; ++e) {
            p_pow = p_pow * p;   /* p^(e+1) */
            term = term + p_pow; /* add to term */
        }

        sigma = sigma * term;          /* multiply contributions   */
        sum_largest = sum_largest + p_pow; /* p_pow is the largest power of this prime */
    }

    /* For n = 1 the loop above is skipped; sigma = 1, sum_largest = 0.  */
    return sigma - sum_largest;
}
