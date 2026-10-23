/*
 * A157488  a(1) = 1; for n > 1, a(n) = product of exponential divisors of n.
 * https://oeis.org/A157488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of all exponential divisors of n
 * Range:     u8 n <= 27, u16 n <= 71, u32 n <= 399, u64 n <= 1763,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, divisor_count_u64, factor_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the definition: a(n) = ∏ primes p_i^(σ(e_i) * d_exp(n) / d(e_i)) */
/* where n = ∏ p_i^e_i, σ(e) is sum of divisors of e, d(e) is number of divisors of e, and */
/* d_exp(n) = ∏ d(e_i).  The function is valid for all n ≥ 1. */

#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/sigma_of.h"
#include "../utils/divisor_count_u64.h"
#include "../utils/big_pow.h"

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
        seq_t di = (seq_t)divisor_count_u64(exps[i]);
        d_exp *= di;
    }

    /* Accumulate the result */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t sigma_e = sigma_of(exps[i]);      /* σ(e_i) */
        seq_t di       = (seq_t)divisor_count_u64(exps[i]); /* d(e_i) */
        /* exponent for prime p_i */
        seq_t exp = sigma_e * d_exp / di;

        /* compute p_i^exp and multiply */
        seq_t term = big_pow((seq_t)primes[i], exp);
        result *= term;
    }

    return result;
}
