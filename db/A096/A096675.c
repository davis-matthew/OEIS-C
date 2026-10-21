/*
 * A096675  a(n) = A096786(n)/2.
 * https://oeis.org/A096675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer u such that 4*u+1 is prime and 2*u+1 is
 *            composite
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define PRIME_MAX 10000000u    /* 10 million – enough for n ≈ 10 000 */

static bool primes[PRIME_MAX + 1];
static bool sieve_done = false;

/* Sieve of Eratosthenes up to PRIME_MAX */
static void init_sieve(void)
{
    if (sieve_done) return;
    size_t i, j;
    for (i = 0; i <= PRIME_MAX; ++i) primes[i] = true;
    primes[0] = primes[1] = false;
    for (i = 2; i * i <= PRIME_MAX; ++i) {
        if (primes[i]) {
            for (j = i * i; j <= PRIME_MAX; j += i)
                primes[j] = false;
        }
    }
    sieve_done = true;
}

/* Dynamic array for cache of sequence values */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_candidate = 1;

/* Grow the cache to accommodate at least 'need' elements */
static void ensure_cache_capacity(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* out of memory – terminate */
    cache = tmp;
    cache_cap = new_cap;
}

/* Public function required by the build */
seq_t a(idx_t n)
{
    init_sieve();

    /* Keep generating terms until we have at least n values */
    while (cache_len < n) {
        /* Candidate u = next_candidate */
        size_t p = (size_t)(4 * next_candidate + 1);
        size_t q = (size_t)(2 * next_candidate + 1);

        /* Ensure indices are within sieve bounds */
        if (p <= PRIME_MAX && q <= PRIME_MAX) {
            if (primes[p] && !primes[q]) {
                ensure_cache_capacity(cache_len + 1);
                cache[cache_len++] = (seq_t)next_candidate;
            }
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
