/*
 * A212493  Let p_n=prime(n), n>=1. Then a(n) is the least prime p which
 *          differs from p_n, for which the intervals (p/2,p_n/2), (p,p_n], if
 *          p<p_n, or the intervals (p_n/2,p/2), (p_n,p], if p>p_n, contain
 *          the same number of primes, and a(n)=0, if no such prime p exists.
 * https://oeis.org/A212493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   computes OEIS sequence A212493. a(n) is the least prime p≠p_n
 *            such that |{primes in (p/2 , p_n/2)}| = |{primes in (p , p_n]}|
 *            if p<p_n, otherwise |{primes in (p_n/2 , p/2)}| = |{primes in
 *            (p_n , p]}| if p>p_n. a(n)=0 iff no such prime exists
 * Range:     u8 n <= 53, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/get_prime.h"

static idx_t          sieve_limit_global = 0;   /* maximum sieved value   */
static uint64_t *     primes_global      = NULL;/* list of primes         */
static size_t        primes_cnt_global  = 0;   /* number of primes       */
static uint32_t *    pi_global          = NULL;/* prefix prime counts    */

/* Rebuild sieve and pi array up to 'limit' (inclusive). */
static void grow_sieve(idx_t limit)
{
    if (limit <= sieve_limit_global) return;  /* already large enough */

    /* Allocate temporary prime flags */
    size_t sz = (size_t)limit + 1;
    uint8_t *is_prime = (uint8_t*)calloc(sz, sizeof(uint8_t));
    if (!is_prime) exit(EXIT_FAILURE);
    for (size_t i = 2; i < sz; ++i) is_prime[i] = 1;

    /* Eratosthenes */
    size_t sqrt_limit = (size_t)sqrt((double)limit);
    for (size_t p = 2; p <= sqrt_limit; ++p) {
        if (!is_prime[p]) continue;
        for (size_t mult = p * p; mult <= limit; mult += p)
            is_prime[mult] = 0;
    }

    /* Estimate number of primes for allocation */
    size_t est = limit / 10 + 100;  /* generous over-estimate           */
    uint64_t *tmp_primes = (uint64_t*)malloc(est * sizeof(uint64_t));
    if (!tmp_primes) exit(EXIT_FAILURE);

    /* Build pi array and prime list */
    uint32_t cum = 0;
    size_t  pi_capacity = sz;               /* one per integer up to limit   */
    uint32_t *tmp_pi = (uint32_t*)malloc(pi_capacity * sizeof(uint32_t));
    if (!tmp_pi) exit(EXIT_FAILURE);

    size_t prime_idx = 0;
    for (size_t i = 0; i < sz; ++i) {
        if (is_prime[i]) {
            ++cum;
            if (prime_idx >= est) {
                est *= 2;
                tmp_primes = (uint64_t*)realloc(tmp_primes, est * sizeof(uint64_t));
                if (!tmp_primes) exit(EXIT_FAILURE);
            }
            tmp_primes[prime_idx++] = i;
        }
        tmp_pi[i] = cum;
    }

    /* Free old data */
    free(primes_global);
    free(pi_global);
    free(is_prime);

    /* Replace with new data */
    primes_global     = tmp_primes;
    primes_cnt_global = prime_idx;
    pi_global         = tmp_pi;
    sieve_limit_global = (idx_t)limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety for indices below offset */

    /* Determine required sieve limit */
    uint64_t p_n = get_prime(n);            /* p_n fits in 32‑bit, cast safely */
    idx_t   required_limit = p_n * 5 + 1000;
    if (required_limit < 2000000) required_limit = 2000000;

    grow_sieve(required_limit);
    /* Ensure p_n itself is inside sieved range (for safety) */
    if (p_n > sieve_limit_global) {
        idx_t new_lim = p_n * 5 + 1000;
        if (new_lim < 2000000) new_lim = 2000000;
        grow_sieve(new_lim);
    }

    size_t pn = (size_t)p_n;
    uint32_t pi_half = pi_global[pn / 2];
    uint32_t pi_pn   = pi_global[pn];

    /* Scan primes in increasing order to find the minimal satisfying prime. */
    for (size_t i = 0; i < primes_cnt_global; ++i) {
        uint64_t p = primes_global[i];
        if (p == p_n) continue;

        if (p < p_n) {
            uint32_t left  = pi_global[(pn - 1) / 2] - pi_global[p / 2];
            uint32_t right = pi_pn - pi_global[p];
            if (left == right) return (seq_t)p;
        } else { /* p > p_n */
            uint32_t left  = pi_global[(p - 1) / 2] - pi_half;
            uint32_t right = pi_global[p] - pi_pn;
            if (left == right) return (seq_t)p;
        }
    }

    /* No satisfying prime found up to the sieved limit – return 0. */
    return 0;
}
