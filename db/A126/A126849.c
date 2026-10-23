/*
 * A126849  Sum over the divisors d of n constrained to cases where all
 *          exponents of the prime factorization of d are prime.
 * https://oeis.org/A126849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{d|n, d > 1, all prime exponents in d’s factorization
 *            are prime} d ; a(1) = 1
 * Range:     u8 n <= 199, u16 n <= 9771, u32 n <= 9771, u64 n <= 9771,
 *            u128 n <= 9771, big any n
 * Helpers:   factor_u64, pow_uint, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"
#include "../utils/seq_mul.h"

/* Small set of prime numbers needed for exponent tests (≤ 61). */
static const uint8_t _prime_exps[] = {
    2, 3, 5, 7, 11, 13, 17, 19,
    23, 29, 31, 37, 41, 43, 47,
    53, 59, 61
};
static const size_t _prime_exps_cnt = sizeof(_prime_exps)/sizeof(_prime_exps[0]);

/* Recursive helper enumerating all divisors with prime exponents. */
static void _dfs(const uint64_t *primes, const uint64_t *exps,
                 size_t prime_cnt, size_t idx,
                 seq_t prod, seq_t *sum)
{
    if (idx == prime_cnt) {
        if (prod != (seq_t)1)                /* skip d = 1 */
            *sum += prod;
        return;
    }

    /* Option f = 0 : skip picking this prime at all. */
    _dfs(primes, exps, prime_cnt, idx + 1, prod, sum);

    /* Options f = prime exponent ≤ exps[idx]. */
    for (size_t j = 0; j < _prime_exps_cnt; ++j) {
        uint64_t p = _prime_exps[j];
        if (p > exps[idx])
            break;
        seq_t p_pow = pow_uint(primes[idx], p);
        seq_t new_prod = seq_mul(prod, p_pow);
        _dfs(primes, exps, prime_cnt, idx + 1, new_prod, sum);
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t p_cnt = 0;
    factor_u64(nn, primes, exps, &p_cnt);

    seq_t sum = 0;
    _dfs(primes, exps, p_cnt, 0, (seq_t)1, &sum);
    return sum;
}
