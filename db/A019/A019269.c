/*
 * A019269  Let Dedekind's psi(m) = product of (p+1)p^(e-1) for primes p,
 *          where p^e is a factor of m. Iterating psi(m) eventually results in
 *          a number of form 2^a*3^b. a(n) is the number of steps to reach
 *          such a number.
 * https://oeis.org/A019269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations of Dedekind's ψ(m) until the result
 *            is of the form 2^a * 3^b
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, is_2pow3pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/is_2pow3pow.h"

/* compute ψ(m) for m <= 2^64-1, result fits in uint64_t for n <= 10000 */
static inline uint64_t psi_uint64(uint64_t m)
{
    uint64_t primes[32], exps[32];
    size_t cnt = 0;
    factor_u64(m, primes, exps, &cnt);

    __uint128_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        __uint128_t p_pow = 1;
        for (uint64_t j = 1; j < e; ++j) p_pow *= p;
        res *= (p + 1);
        res *= p_pow;
    }
    return (uint64_t)res;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    uint64_t cur = (uint64_t)n;
    idx_t steps = 0;

    while (!is_2pow3pow(cur)) {
        cur = psi_uint64(cur);
        ++steps;
    }
    return (seq_t)steps;
}
