/*
 * A083262  a(n) = sigma(A046523(n)), sum of divisors of the least number with
 *          the same prime signature as n.
 * https://oeis.org/A083262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma( least integer with the same prime signature as n )
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sum-of-divisors of the smallest integer that
   has exactly the same multiset of exponents as n. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

/* The first thirty primes are sufficient for all 64‑bit
   integers (the product of the first 30 primes far exceeds 2^64). */
static const uint64_t _small_primes[30] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL,
    29ULL, 31ULL, 37ULL, 41ULL, 43ULL, 47ULL, 53ULL, 59ULL,
    61ULL, 67ULL, 71ULL, 73ULL, 79ULL, 83ULL, 89ULL, 97ULL,
    101ULL, 103ULL, 107ULL, 109ULL, 113ULL
};

/* Returns a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return 0;    /* undefined for n < 1 */

    /* n == 1 is a trivial case */
    if (n == (idx_t)1) return (seq_t)1;

    /* Factor n and collect exponents */
    uint64_t primes[64];
    uint64_t expos[64];
    size_t   cnt = 0;
    factor_u64((uint64_t)n, primes, expos, &cnt);

    /* Sort exponents in descending order to minimise the product */
    for (size_t i = 0; i < cnt; ++i)
        for (size_t j = i + 1; j < cnt; ++j)
            if (expos[i] < expos[j]) {
                uint64_t t = expos[i];
                expos[i] = expos[j];
                expos[j] = t;
            }

    seq_t result = 1;

    /* For each exponent, compute the sigma factor for the
       corresponding smallest prime. */
    for (size_t i = 0; i < cnt; ++i) {
        seq_t pow_term    = big_pow((seq_t)_small_primes[i],
                                    (idx_t)(expos[i] + 1ULL));
        seq_t numerator   = pow_term - (seq_t)1;
        seq_t denominator = (seq_t)(_small_primes[i] - 1ULL);
        seq_t sigma_factor = numerator / denominator;
        result *= sigma_factor;
    }

    return result;
}
