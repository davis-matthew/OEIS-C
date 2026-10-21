/*
 * A200811  Iterate k -> d(k) until an odd prime is reached, starting from the
 *          n-th composite.
 * https://oeis.org/A200811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3870, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small, make_bool_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A200811:
   iterate d(k) (number of divisors) until an odd prime is reached,
   starting from the n‑th composite number (composites: 4,6,8,9,10,12,14,15,16,…). */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"
#include "../utils/make_bool_array.h"

/* Global storage for primes and composites, lazily expanded. */
static idx_t *primes = NULL;
static size_t primes_cnt = 0;
static idx_t *composites = NULL;
static size_t composites_cnt = 0;

/* Build primes up to `limit` and collect the first `target_n` composites. */
static void build_primes_and_composites(idx_t target_n, idx_t limit)
{
    bool *is_prime = make_bool_array(limit);

    /* Activate all entries first */
    for (idx_t i = 0; i <= limit; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    /* Eratosthenes sieve */
    for (idx_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;

    /* Count primes to allocate array */
    size_t pcnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++pcnt;

    /* Store primes */
    idx_t *new_primes = (idx_t*)malloc(pcnt * sizeof(idx_t));
    if (!new_primes) abort();
    size_t pidx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            new_primes[pidx++] = i;

    /* Collect composites up to target_n */
    size_t ccnt = 0;
    idx_t *new_comps = (idx_t*)malloc(target_n * sizeof(idx_t));
    if (!new_comps) abort();
    for (idx_t i = 4; i <= limit && ccnt < target_n; ++i)
        if (!is_prime[i])
            new_comps[ccnt++] = i;

    /* Replace old tables */
    free(primes);
    primes = new_primes;
    primes_cnt = pcnt;

    free(composites);
    composites = new_comps;
    composites_cnt = ccnt;

    free(is_prime);
}

/* Ensure that at least n composites are available. */
static void ensure_composites(idx_t n)
{
    if (composites_cnt >= n)
        return;

    idx_t limit = 100;
    while ((idx_t)composites_cnt < n) {
        if (limit < n * 2)
            limit = n * 2;
        build_primes_and_composites(n, limit);
        if ((idx_t)composites_cnt < n)
            limit *= 2;  /* grow and try again */
    }
}

/* Compute τ(k) for a 64‑bit integer k.  k is guaranteed to be composite. */
static idx_t tau_of_idx(idx_t k)
{
    idx_t tau = 1;
    idx_t tmp = k;
    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= tmp; ++i) {
        idx_t p = primes[i];
        if (tmp % p == 0) {
            idx_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            tau *= (e + 1);
        }
    }
    if (tmp > 1)             /* remaining prime factor */
        tau *= 2;
    return tau;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined for n<1; harmless */

    ensure_composites(n);
    seq_t k = composites[n - 1];    /* nth composite */

    for (;;) {
        idx_t kk = (idx_t)k;      /* k fits in idx_t during iterations */
        idx_t t  = tau_of_idx(kk);
        if ((t & 1) && is_prime_small((seq_t)t))
            return (seq_t)t;      /* odd prime reached */
        k = (seq_t)t;             /* continue with τ(k) */
    }
}
