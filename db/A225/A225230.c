/*
 * A225230  In the canonical prime factorization of n: (number of distinct
 *          primes) minus (largest prime exponent).
 * https://oeis.org/A225230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A001221(n) - A051903(n)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   count_distinct_primes, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/count_distinct_primes.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    uint64_t num = (uint64_t)n;

    /* Number of distinct prime factors */
    int distinct = count_distinct_primes(num);

    /* Factorize n into primes and exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64(num, primes, exps, &cnt);

    /* Find the largest exponent in the factorization */
    int max_exp = 0;
    for (size_t i = 0; i < cnt; ++i) {
        int e = (int)exps[i];
        if (e > max_exp) {
            max_exp = e;
        }
    }

    /* Return the difference */
    return (seq_t)distinct - (seq_t)max_exp;
}
