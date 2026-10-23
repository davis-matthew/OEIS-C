/*
 * A175418  Complement of A175397, where A175397 = numbers such that both
 *          arithmetic means of distinct and all prime factors are not
 *          integers.
 * https://oeis.org/A175418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer that is NOT in A175397 (i.e. n's
 *            arithmetic mean of distinct prime factors is an integer or the
 *            arithmetic mean of all prime factors is an integer)
 * Range:     u8 n <= 164, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* ---------- prime generation utilities ---------- */
static uint32_t *primes = NULL;
static size_t primes_size = 0;

static void ensure_primes_up_to(uint64_t n)
{
    if (primes != NULL && primes_size > 0 && primes[primes_size - 1] >= n)
        return;

    if (n < 2)
        return;

    /* Simple sieve up to n */
    size_t limit = (size_t)(n + 1);
    unsigned char *is_prime = (unsigned char *)malloc(limit);
    if (!is_prime)
        return; /* allocation failure; silently fail – unlikely */

    for (size_t i = 0; i < limit; ++i)
        is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    uint64_t sq = (uint64_t)sqrt((long double)n) + 1;
    for (uint64_t p = 2; p <= sq; ++p) {
        if (is_prime[p]) {
            for (uint64_t j = p * p; j < limit; j += p)
                is_prime[j] = 0;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (size_t i = 2; i < limit; ++i)
        if (is_prime[i])
            ++cnt;

    /* Allocate and fill */
    uint32_t *new_primes = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!new_primes) {
        free(is_prime);
        return;
    }
    size_t idx = 0;
    for (size_t i = 2; i < limit; ++i)
        if (is_prime[i]) {
            new_primes[idx++] = (uint32_t)i;
        }

    free(is_prime);
    if (primes)
        free(primes);
    primes = new_primes;
    primes_size = cnt;
}

/* ---------- check if a number satisfies the condition ---------- */
static bool passes(uint64_t m)
{
    if (m <= 1)
        return false; /* sequence starts from 2 */

    uint64_t temp = m;
    uint64_t sum_distinct = 0, sum_all = 0;
    uint64_t count_all = 0, distinct_count = 0;

    uint64_t sqrt_m = (uint64_t)sqrt((long double)m) + 1;
    ensure_primes_up_to(sqrt_m);

    for (size_t i = 0; i < primes_size && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        uint32_t p = primes[i];
        if (temp % p == 0) {
            uint64_t cnt = 0;
            do {
                temp /= p;
                ++cnt;
                ++count_all;
                sum_all += p;
            } while (temp % p == 0);
            sum_distinct += p;
            ++distinct_count;
        }
    }

    if (temp > 1) { /* remaining factor is prime */
        sum_all += temp;
        ++count_all;
        sum_distinct += temp;
        ++distinct_count;
    }

    if (distinct_count == 0)
        return false;

    return (sum_distinct % distinct_count == 0) ||
           (sum_all % count_all == 0);
}

/* ---------- main sequence routine ---------- */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;
    static uint64_t next_candidate = 2; /* next number to test */

    if (!cache) {
        cache_cap = 131072; /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0; /* allocation failure; unlikely */
    }

    while (cache_len < n) {
        while (!passes(next_candidate))
            ++next_candidate;

        /* grow if needed */
        if (cache_len >= cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!cache)
                return (seq_t)0; /* allocation failure; unlikely */
        }

        cache[cache_len++] = (seq_t)next_candidate;
        ++next_candidate;
    }

    return cache[n - 1];
}
