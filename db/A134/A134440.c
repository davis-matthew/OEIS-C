/*
 * A134440  a(0)=1; for n > 0, a(n) = a(n-1) + prime(n) if a(n-1) is odd, else
 *          a(n) = a(n-1)/2.
 * https://oeis.org/A134440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>0, a(n)=a(n-1)+prime(n) if a(n-1) is odd, * else
 *            a(n)=a(n-1)/2 * primes are indexed starting at 1 (prime(1)=2),
 *            so prime(n)=primes[n-1]
 * Range:     u8 n <= 44, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --------------------------------------------------------------- */
/* Prime generation (sieve) – up to a fixed upper bound sufficient   */
/* for n <= 10000  (prime(10000) = 104729 < 200000).                 */
/* --------------------------------------------------------------- */
#define PRIME_SIEVE_LIMIT 200000   /* enough for prime(10000) */
#define PRIME_MAX_COUNT   19000   /* pi(200000) ≈ 17984 */

static uint64_t primes[PRIME_MAX_COUNT];
static size_t  primes_cnt = 0;
static  bool   primes_ready = false;

static void init_primes(void)
{
    if (primes_ready) return;

    /* Simple Sieve of Eratosthenes */
    char *is_prime = (char *)malloc(PRIME_SIEVE_LIMIT + 1);
    if (!is_prime) exit(EXIT_FAILURE);
    for (size_t i = 0; i <= PRIME_SIEVE_LIMIT; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (size_t i = 2; i * i <= PRIME_SIEVE_LIMIT; ++i)
    {
        if (is_prime[i])
        {
            for (size_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    /* store primes into static array */
    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT && primes_cnt < PRIME_MAX_COUNT; ++i)
    {
        if (is_prime[i]) primes[primes_cnt++] = (uint64_t)i;
    }

    free(is_prime);
    primes_ready = true;
}

/* --------------------------------------------------------------- */
/* Sequence a(n) with memoisation                                  */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* initialise primes on first call */
    if (!primes_ready) init_primes();

    /* static cache for a-values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* expand cache up to n if necessary */
    if (n >= cache_len)
    {
        size_t new_len = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);
        cache = new_cache;

        /* initialise first term if cache was empty */
        if (cache_len == 0) {
            cache[0] = (seq_t)1;
            cache_len = 1;
        }

        /* compute missing terms */
        for (size_t i = cache_len; i <= n; ++i)
        {
            seq_t prev = cache[i - 1];
            if ((prev & 1) != 0)          /* odd */
                cache[i] = prev + (seq_t)primes[i - 1];  /* prime(i) */
            else                          /* even */
                cache[i] = prev >> 1;     /* division by 2 */
        }
        cache_len = new_len;
    }

    return cache[n];
}
