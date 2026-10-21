/*
 * A089247  a(n) is the smallest number obtainable by permuting the exponents
 *          in the prime factorization of n.
 * https://oeis.org/A089247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest number with the same squarefree kernel and prime
 *            signature as n
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, cmp_uint64_asc, cmp_uint64_desc, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"
#include "../utils/cmp_uint64_asc.h"
#include "../utils/cmp_uint64_desc.h"

seq_t a(idx_t n)
{
    if (n < 2) {
        return (seq_t)1;
    }

    /* maximum number of distinct prime factors in a 64‑bit integer */
    const size_t MAX_FACTORS = 64;
    uint64_t primes[MAX_FACTORS];
    uint64_t exps[MAX_FACTORS];
    size_t count = 0;

    factor_u64((uint64_t)n, primes, exps, &count);
    if (count == 0) {
        return (seq_t)1;
    }

    /* sort primes ascending to use the smallest primes first */
    qsort(primes, count, sizeof(uint64_t), cmp_uint64_asc);

    /* copy exponents for independent sorting */
    uint64_t exps_desc[MAX_FACTORS];
    for (size_t i = 0; i < count; ++i) {
        exps_desc[i] = exps[i];
    }
    /* sort exponents descending (largest exponent to smallest prime) */
    qsort(exps_desc, count, sizeof(uint64_t), cmp_uint64_desc);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < count; ++i) {
        seq_t prime = (seq_t)primes[i];
        seq_t exp   = (seq_t)exps_desc[i];
        seq_t pow   = big_pow(prime, exp);
        result = result * pow;
    }

    return result;
}
