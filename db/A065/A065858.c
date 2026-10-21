/*
 * A065858  m-th composite number c(m) = A002808(m), where m is the n-th prime
 *          number: a(n) = A002808(A000040(n)).
 * https://oeis.org/A065858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = c(p_n), where p_n is the n-th prime and c(m) is the m-th
 *            composite number. The function generates primes and composites
 *            on demand and returns the p_n‑th composite
 * Range:     u8 n <= 46, u16 n <= 5962, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

static idx_t cur_limit = 0;
static idx_t *primes = NULL;
static size_t primes_cnt = 0;
static seq_t *composites = NULL;
static size_t comps_cnt = 0;

/* Rebuild the sieve up to new_limit and fill the global arrays. */
static void rebuild_sieve(idx_t new_limit)
{
    if (new_limit <= cur_limit) return;

    /* Allocate a boolean sieve array */
    size_t sz = (size_t)new_limit + 1;
    bool *is_prime = (bool *)calloc(sz, sizeof(bool));
    if (!is_prime) abort();

    for (size_t i = 0; i < sz; ++i) is_prime[i] = true;
    if (new_limit >= 0) {
        is_prime[0] = false;
        if (new_limit >= 1) is_prime[1] = false;
    }

    /* Sieve of Eratosthenes */
    for (idx_t i = 2; i * i <= new_limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= new_limit; j += i)
                is_prime[j] = false;
        }
    }

    /* Build prime array */
    idx_t *new_primes = NULL;
    size_t new_primes_cnt = 0;
    if (new_limit >= 2) {
        new_primes = (idx_t *)malloc(sz * sizeof(idx_t));
        if (!new_primes) abort();
        for (idx_t i = 2; i <= new_limit; ++i)
            if (is_prime[i])
                new_primes[new_primes_cnt++] = i;
    }

    /* Build composite array */
    seq_t *new_composites = NULL;
    size_t new_comps_cnt = 0;
    if (new_limit >= 4) {
        new_composites = (seq_t *)malloc(sz * sizeof(seq_t));
        if (!new_composites) abort();
        for (idx_t i = 4; i <= new_limit; ++i)
            if (!is_prime[i])
                new_composites[new_comps_cnt++] = (seq_t)i;
    }

    free(is_prime);
    free(primes);
    free(composites);

    primes = new_primes;
    primes_cnt = new_primes_cnt;
    composites = new_composites;
    comps_cnt = new_comps_cnt;
    cur_limit = new_limit;
}

/* Ensure that we have at least needed_primes primes
   and needed_composites composites. */
static void ensure_limit(idx_t needed_primes, idx_t needed_composites)
{
    /* Rough upper bound for the needed prime (n*(log n + log log n) + 3). */
    idx_t limit_for_primes = 15;
    if (needed_primes >= 6) {
        double n = (double)needed_primes;
        double bound = n * (log(n) + log(log(n))) + 3.0;
        if (bound > (double)SIZE_MAX) bound = (double)SIZE_MAX;
        limit_for_primes = (idx_t)bound;
    }

    idx_t limit = limit_for_primes;
    if (needed_composites > limit) limit = needed_composites + 100;

    rebuild_sieve(limit);

    /* Double the limit until we have enough numbers. */
    while (primes_cnt < (size_t)needed_primes ||
           comps_cnt < (size_t)needed_composites) {
        rebuild_sieve(cur_limit * 2 + 1000);
    }
}

/* Required sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; undefined for n=0 */

    /* Make sure we have at least n primes. */
    ensure_limit(n, 0);

    /* Retrieve the n-th prime (1‑based). */
    idx_t p_n = primes[n - 1];

    /* Make sure we have at least p_n composites. */
    ensure_limit(n, p_n);

    /* Return the p_n‑th composite number. */
    return composites[p_n - 1];
}
