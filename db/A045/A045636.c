/*
 * A045636  Numbers of the form p^2 + q^2, with p and q primes.
 * https://oeis.org/A045636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045636
   Numbers of the form p^2 + q^2 with p and q primes.
   This function builds a cache of all distinct sums up to a prime
   limit that is large enough to supply at least n terms. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Comparator for qsort – works for both built‑in integer types and
   arbitrary‑precision integer types that provide operator<. */
static int seq_cmp(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Static cache of computed values */
static seq_t *cache = NULL;               /* sorted unique sums */
static size_t cache_len = 0;              /* number of cached terms */
static uint64_t cache_prime_limit = 0;    /* highest prime limit used */

/* Build all primes up to prime_limit and fill the cache with
   all distinct sums p^2 + q^2 (with p <= q). */
static void build_cache(uint64_t prime_limit)
{
    /* --- Sieve of Eratosthenes --- */
    bool *is_prime = (bool *)malloc((prime_limit + 1) * sizeof(bool));
    if (!is_prime) abort();
    for (uint64_t i = 0; i <= prime_limit; ++i) is_prime[i] = true;
    if (prime_limit >= 0) { is_prime[0] = false; }
    if (prime_limit >= 1) { is_prime[1] = false; }
    for (uint64_t i = 2; i * i <= prime_limit; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= prime_limit; j += i) {
                is_prime[j] = false;
            }
        }
    }

    /* Count primes, store them */
    size_t primes_cnt = 0;
    for (uint64_t i = 2; i <= prime_limit; ++i) {
        if (is_prime[i]) ++primes_cnt;
    }
    uint64_t *primes = (uint64_t *)malloc(primes_cnt * sizeof(uint64_t));
    if (!primes) abort();
    size_t idx = 0;
    for (uint64_t i = 2; i <= prime_limit; ++i) {
        if (is_prime[i]) primes[idx++] = i;
    }

    /* Generate all p^2 + q^2 (p <= q) */
    size_t pair_count = primes_cnt * (primes_cnt + 1) / 2;
    seq_t *sums = (seq_t *)malloc(pair_count * sizeof(seq_t));
    if (!sums) abort();
    size_t pos = 0;
    for (size_t i = 0; i < primes_cnt; ++i) {
        seq_t pi_sq = (seq_t)primes[i] * primes[i];
        for (size_t j = i; j < primes_cnt; ++j) {
            sums[pos++] = pi_sq + (seq_t)primes[j] * primes[j];
        }
    }

    /* Sort sums */
    qsort(sums, pair_count, sizeof(seq_t), seq_cmp);

    /* Deduplicate into new_cache */
    seq_t *new_cache = (seq_t *)malloc(pair_count * sizeof(seq_t));
    if (!new_cache) abort();
    size_t uniq = 0;
    for (size_t i = 0; i < pair_count; ++i) {
        if (i == 0 || sums[i] != sums[i - 1]) {
            new_cache[uniq++] = sums[i];
        }
    }

    /* Replace old cache */
    free(cache);
    cache = new_cache;
    cache_len = uniq;
    cache_prime_limit = prime_limit;

    free(primes);
    free(is_prime);
    free(sums);
}

/* Ensure that the cache contains at least n terms. */
static void ensure_cache_up_to(idx_t n)
{
    if (cache_len >= (size_t)n) return;
    uint64_t limit = (cache_prime_limit > 2000) ? cache_prime_limit : 2000;
    while (true) {
        build_cache(limit);
        if (cache_len >= (size_t)n) break;
        limit *= 2;
    }
}

/* Return the n-th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 1 */
    ensure_cache_up_to(n);
    return cache[n - 1];
}
