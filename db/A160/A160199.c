/*
 * A160199  Product of non-exponential divisors of n.
 * https://oeis.org/A160199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of non‑exponential divisors of n
 * Range:     u8 n <= 23, u16 n <= 41, u32 n <= 119, u64 n <= 359,
 *            u128 n <= 839, big any n
 * Helpers:   factor_u64, seq_mul, seq_pow, tau_sum_divisors_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"
#include "../utils/tau_sum_divisors_uint64.h"

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)1;
    }

    uint64_t primes[32];
    uint64_t exps[32];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);
    if (pcnt == 0) {
        return (seq_t)1;
    }

    /* τ(n) – number of divisors of n */
    uint64_t tau = 1;
    for (size_t i = 0; i < pcnt; ++i) {
        tau *= (exps[i] + 1);
    }

    /* For each prime exponent: τ(exp e), Σ(divisors of e) */
    uint64_t tau_prime[32];
    uint64_t sum_prime[32];
    uint64_t tau_exp = 1;
    for (size_t i = 0; i < pcnt; ++i) {
        tau_sum_divisors_uint64(exps[i], &tau_prime[i], &sum_prime[i]);
        tau_exp *= tau_prime[i];
    }

    /* Build product of non‑exponential divisors */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t Ti = (tau * exps[i]) / 2;                     /* exponent of pi in product of all divisors */
        uint64_t exp_i = (tau_exp / tau_prime[i]) * sum_prime[i]; /* exponent of pi in product of exponential divisors */
        uint64_t diff_i = Ti - exp_i;                         /* exponent of pi in product of non‑exponential divisors */
        if (diff_i == 0) {
            continue;
        }
        seq_t term = seq_pow((seq_t)primes[i], (idx_t)diff_i);
        result = seq_mul(result, term);
    }

    return result;
}
