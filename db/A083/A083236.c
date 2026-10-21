/*
 * A083236  First order recursion: a(0)=2; a(n) = prime(n) - a(n-1).
 * https://oeis.org/A083236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) - a(n-1), a(0) = 2 (prime(n) is the nth prime
 *            with prime(1) = 2)
 * Range:     u8 n <= 86, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Sequence value type and index type are defined by the build environment. */
static seq_t *cache = NULL;          /* computed a(n) values */
static size_t cache_cap = 0;
static size_t cache_size = 0;

static uint32_t *prime_list = NULL;  /* primes[0] = 2, primes[1] = 3, ...   */
static size_t prime_cap = 0;
static size_t prime_count = 0;
static idx_t  prime_limit  = 0;

/* Ensure that prime_list has at least n primes (prime(n) exists). */
static void ensure_primes(idx_t n)
{
    if (prime_count >= (size_t)n)
        return;

    /* Rough upper bound for the nth prime: n*20 is safe for n up to at least 100 000. */
    idx_t limit = (idx_t)(n * 20);
    if (limit < 100)
        limit = 100;

    /* Increase limit until we have enough primes. */
    while (prime_count < (size_t)n) {
        /* First perform the sieve up to 'limit'. */
        bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
        if (!is_prime) abort();  /* allocation failure */

        for (idx_t i = 0; i <= limit; ++i)
            is_prime[i] = true;
        is_prime[0] = is_prime[1] = false;

        for (idx_t i = 2; (i * i) <= limit; ++i) {
            if (is_prime[i]) {
                for (idx_t j = i * i; j <= limit; j += i)
                    is_prime[j] = false;
            }
        }

        /* Count primes. */
        size_t cnt = 0;
        for (idx_t i = 2; i <= limit; ++i)
            if (is_prime[i]) ++cnt;

        /* Allocate prime array. */
        uint32_t *primes = (uint32_t *)malloc(cnt * sizeof(uint32_t));
        if (!primes) abort();

        size_t idx = 0;
        for (idx_t i = 2; i <= limit; ++i)
            if (is_prime[i]) primes[idx++] = (uint32_t)i;

        free(is_prime);

        /* Replace old prime list. */
        if (prime_list)
            free(prime_list);
        prime_list = primes;
        prime_count = cnt;
        prime_limit = limit;

        /* If still not enough, double the limit and repeat. */
        if (prime_count < (size_t)n)
            limit *= 2;
    }
}

/* Ensure that cache[0 .. n] are computed. */
static void ensure_cache(idx_t n)
{
    if (cache_size > n)
        return;

    /* Ensure enough capacity. */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
        while (new_cap <= (size_t)n)
            new_cap *= 2;
        seq_t *new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_buf) abort();
        cache = new_buf;
        cache_cap = new_cap;
    }

    /* Compute missing terms. */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)2;
        } else {
            seq_t prev   = cache[i - 1];
            seq_t prime  = (seq_t)prime_list[i - 1];  /* prime(i) */
            cache[i] = prime - prev;
        }
    }
    cache_size = n + 1;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)2;

    ensure_primes(n);
    ensure_cache(n);
    return cache[n];
}
