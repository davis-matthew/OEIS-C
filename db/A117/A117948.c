/*
 * A117948  Sum of the divisors of pentagonal numbers.
 * https://oeis.org/A117948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sigma(n*(3*n-1)/2) (sum of divisors of the n-th
 *            pentagonal number)
 * Range:     u8 n <= 5, u16 n <= 131, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    /* compute the n-th pentagonal number:  m = n*(3*n-1)/2  */
    unsigned __int128 prod = (unsigned __int128)n * ((unsigned __int128)3 * n - 1);
    uint64_t m = (uint64_t)(prod / 2);   /* fits in 64 bits for all n we handle */

    /* factor m into primes */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    if (m == 0)           /* guard against accidental zero (should not happen) */
        return (seq_t)0;
    factor_u64(m, primes, exps, &cnt);

    /* compute sigma(m) = product over prime powers of (p^(a+1)-1)/(p-1) */
    seq_t sigma = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        unsigned int a = (unsigned int)exps[i];

        /* compute p^(a+1) */
        seq_t power = (seq_t)1;
        for (unsigned int j = 0; j < a + 1; ++j)
            power = power * p;

        /* ratio = (p^(a+1)-1)/(p-1) */
        seq_t numerator   = power - (seq_t)1;
        seq_t denominator = p - (seq_t)1;
        seq_t term = numerator / denominator;

        sigma = sigma * term;
    }
    return sigma;
}
