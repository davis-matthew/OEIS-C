/*
 * A068992  Composite numbers k such that Sum_{d|k} sigma(d)/tau(d) is an
 *          integer.
 * https://oeis.org/A068992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the nth composite integer k such that Σ_{d|k} σ(d)/τ(d)
 *            is an integer. This implementation searches composite numbers,
 *            checks the condition, and caches results so that subsequent
 *            calls are fast
 * Range:     u8 n <= 9, u16 n <= 317, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Utilities provided by the build system. */
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */
#include "../utils/is_prime.h"     /* bool is_prime(idx_t n) */
#include "../utils/gcd.h"          /* seq_t gcd(seq_t a, seq_t b) */

/* Compute σ(d) and τ(d) for a single divisor d.  The result is
   returned via the out parameters.  For d = 1, σ = 1 and τ = 1. */
static void
sigma_tau_of(uint64_t d, seq_t *sigma_out, seq_t *tau_out)
{
    if (d == 1) {
        *sigma_out = (seq_t)1;
        *tau_out   = (seq_t)1;
        return;
    }

    uint64_t primes[32];
    uint64_t exps[32];
    size_t pf_cnt = 0;
    factor_u64(d, primes, exps, &pf_cnt);

    seq_t sigma = 1;
    seq_t tau   = 1;
    for (size_t i = 0; i < pf_cnt; ++i) {
        /* Compute p^(e+1) */
        seq_t pow_p = 1;
        for (uint64_t e = 0; e < exps[i] + 1; ++e)
            pow_p *= (seq_t)primes[i];

        /* term = (p^(e+1)-1)/(p-1) */
        seq_t term = (pow_p - 1) / (primes[i] - 1);
        sigma *= term;
        tau   *= (seq_t)(exps[i] + 1);
    }

    *sigma_out = sigma;
    *tau_out   = tau;
}

/* Return true if composite k satisfies the OEIS condition. */
static bool
condition_is_met(uint64_t k)
{
    /* For a cumulative rational sum num/den.  Use the seq_t type. */
    seq_t num = 0;
    seq_t den = 1;

    /* Generate all divisors of k using its prime factorisation. */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t pf_cnt = 0;
    factor_u64(k, primes, exps, &pf_cnt);

    /* Capacity for divisors – the number of divisors is at most
       product of (e_i + 1).  For 64‑bit numbers this is below 10^5. */
    size_t dcnt = 1;
    uint64_t divisors[1024];
    divisors[0] = 1;   /* divisor 1 */

    for (size_t i = 0; i < pf_cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        size_t cur = dcnt;
        uint64_t mult = 1;
        for (uint64_t exp = 1; exp <= e; ++exp) {
            mult *= p;
            for (size_t j = 0; j < cur; ++j)
                divisors[dcnt++] = divisors[j] * mult;
        }
    }

    for (size_t i = 0; i < dcnt; ++i) {
        uint64_t d = divisors[i];
        seq_t sigma, tau;
        sigma_tau_of(d, &sigma, &tau);

        /* Add sigma/tau to num/den. */
        seq_t new_num = num * tau + den * sigma;
        seq_t new_den = den * tau;
        seq_t g = gcd(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
    }

    /* The sum is an integer iff the denominator divides the numerator. */
    return (num % den) == 0;
}

/* Cache of sequence values. */
static seq_t *cache   = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static uint64_t next_candidate = 4;   /* next integer to test (composite only) */

/* Append a value to the dynamic cache. */
static void
cache_append(seq_t val)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* a(idx_t n) -- returns the n‑th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for offset 1 */

    /* Grow the cache until it contains at least n entries. */
    while (cache_len < (size_t)n) {
        /* Skip primes and 1, only test composites. */
        while (is_prime(next_candidate)) ++next_candidate;
        if (condition_is_met(next_candidate)) {
            cache_append((seq_t)next_candidate);
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
