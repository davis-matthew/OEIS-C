/*
 * A023825  Sum of exponents in prime-power factorization of C(3n,n+3).
 * https://oeis.org/A023825
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of exponents in the prime‐factorization of C(3n, n+3)
 * Range:     u8 n <= 87, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _expo_factorial, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Requires: seq_t, idx_t, and build_primes_upto from ../utils/build_primes_upto.h */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/_expo_factorial.h"

static seq_t _bigomega_binomial(idx_t n)
{
    seq_t three_n = (seq_t)3 * n;
    seq_t n_plus_3 = (seq_t)n + 3;
    seq_t two_n_minus_3 = (seq_t)2 * n - 3;

    /* Generate all primes up to 3n */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto((idx_t)(three_n), &primes, &prime_cnt);

    if (!primes || prime_cnt == 0) {
        if (primes) free(primes);
        return 0;
    }

    seq_t exp_sum = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t e = _expo_factorial(three_n, p)
                - _expo_factorial(n_plus_3,  p)
                - _expo_factorial(two_n_minus_3, p);
        exp_sum += e;
    }

    free(primes);
    return exp_sum;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;
    return _bigomega_binomial(n);
}
