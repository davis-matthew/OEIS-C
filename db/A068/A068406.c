/*
 * A068406  Numbers n such that n and 2n+1 have the same number of prime
 *          divisors.
 * https://oeis.org/A068406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth term of A068406: numbers n such that the number of distinct
   prime divisors of n equals that of 2n+1.  The function searches
   candidates in increasing order, caches the results, and returns the
   requested term.  For performance it uses a sieve of primes up to a fixed
   limit and counts distinct prime factors. */
#include <stdint.h>
#include "../utils/build_primes_upto.h"

static uint64_t *primes = NULL;          /* list of small primes */
static size_t   prime_cnt = 0;           /* number of primes in the list */
static const uint64_t PRIME_LIMIT = 200000;  /* sufficient for n ≤ 10 000 */

static seq_t *cache = NULL;             /* cached terms of the sequence */
static size_t cache_len = 0;            /* number of terms stored */
static size_t cache_cap = 0;            /* capacity of the cache array */
static uint64_t last_checked = 1;       /* largest n examined so far */

/* Count distinct prime factors of n (Ω omitted). */
static uint64_t distinct_omega_uint64(uint64_t n)
{
    uint64_t cnt = 0;
    uint64_t t = n;
    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= t; ++i) {
        uint64_t p = primes[i];
        if (t % p == 0) {
            ++cnt;
            while (t % p == 0)
                t /= p;
        }
    }
    if (t > 1)
        ++cnt;
    return cnt;
}

/* Lazily initialize the global prime list. */
static void init_primes(void)
{
    if (primes)
        return;
    build_primes_upto(PRIME_LIMIT, &primes, &prime_cnt);
}

/* Ensure the cache has capacity for at least new_cap terms. */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    size_t cap = cache_cap ? cache_cap * 2 : 16;
    while (cap < new_cap)
        cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = cap;
}

/* Extend the cached terms until at least target terms are available. */
static void ensure_cached(idx_t target)
{
    if (cache_len >= (size_t)target)
        return;
    init_primes();
    while (cache_len < (size_t)target) {
        ++last_checked;
        uint64_t cand = last_checked;
        if (distinct_omega_uint64(cand) == distinct_omega_uint64(2 * cand + 1)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)cand;
        }
    }
}

/* Return the nth term of A068406. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    ensure_cached(n);
    return cache[n - 1];
}
