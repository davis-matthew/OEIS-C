/*
 * A157488  a(1) = 1; for n > 1, a(n) = product of exponential divisors of n.
 * https://oeis.org/A157488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of all exponential divisors of n
 * Range:     u8 n <= 27, u16 n <= 71, u32 n <= 399, u64 n <= 1763,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9141 (2026-11-01)
 * Helpers:   factor_u64, ipow, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the definition: a(n) = ∏ primes p_i^(σ(e_i) * d_exp(n) / d(e_i)) */
/* where n = ∏ p_i^e_i, σ(e) is sum of divisors of e, d(e) is number of divisors of e, and */
/* d_exp(n) = ∏ d(e_i).  The function is valid for all n ≥ 1. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow.h"
#include "../utils/num_divisors.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* handle the trivial case */
    if (n == 1)
        return (seq_t)1;

    /* Factorization of n */
    /* The number of distinct prime factors of a 64‑bit integer is at most 16,
       so 32 is a safe upper bound for the buffer sizes. */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute d_exp(n) = Π d(e_i) as seq_t */
    seq_t d_exp = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t di = (seq_t)num_divisors(exps[i]);
        d_exp *= di;
    }

    /* Accumulate the result */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t sigma_e = sigma_of(exps[i]);      /* σ(e_i) */
        seq_t di       = (seq_t)num_divisors(exps[i]); /* d(e_i) */
        /* exponent for prime p_i */
        seq_t exp = sigma_e * d_exp / di;

        /* compute p_i^exp and multiply */
        seq_t term = ipow((seq_t)primes[i], exp);
        result *= term;
    }

    return result;
}
