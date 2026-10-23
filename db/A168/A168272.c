/*
 * A168272  a(n) = abs(prime(prime(n))-prime(nonprime(n))).
 * https://oeis.org/A168272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = abs(prime(prime(n)) - prime(nonprime(n)))
 * Range:     u8 n <= 16, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds a small prime sieve that grows as needed
   to provide the required primes and non‑primes for any n >= 1. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ---------- static state ---------- */
struct cache_t
{
    size_t limit;          /* current sieve upper bound */
    uint32_t *primes;      /* array of primes, 1‑based */
    size_t primes_cnt;     /* number of primes stored */
    uint32_t *nonprimes;   /* array of non‑primes (includes 1), 1‑based */
    size_t nonprimes_cnt;  /* number of nonprimes stored */
} cache = {0, NULL, 0, NULL, 0};

/* ---------- rebuild sieve up to 'limit' (inclusive) ---------- */
static void rebuild_sieve(size_t limit)
{
    /* free old data */
    free(cache.primes);
    free(cache.nonprimes);

    /* allocate is_prime bitmap */
    uint8_t *is_prime = (uint8_t *)malloc((limit + 1) * sizeof(uint8_t));
    if (!is_prime) exit(1);
    memset(is_prime, 1, limit + 1);
    if (limit >= 0) is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;

    /* classic sieve of Eratosthenes, using i*i loop guard */
    for (size_t i = 2; i * i <= limit; ++i)
    {
        if (is_prime[i])
        {
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* first pass: count primes and non‑primes */
    size_t p_count = 0, np_count = 0;
    for (size_t i = 1; i <= limit; ++i)
    {
        if (i == 1 || !is_prime[i]) ++np_count;
        else ++p_count;
    }

    cache.primes = (uint32_t *)malloc(p_count * sizeof(uint32_t));
    cache.nonprimes = (uint32_t *)malloc(np_count * sizeof(uint32_t));
    if (!cache.primes || !cache.nonprimes) exit(1);

    /* second pass: store values */
    size_t p_idx = 0, np_idx = 0;
    for (size_t i = 1; i <= limit; ++i)
    {
        if (i == 1 || !is_prime[i])
            cache.nonprimes[np_idx++] = (uint32_t)i;
        else
            cache.primes[p_idx++] = (uint32_t)i;
    }

    cache.primes_cnt = p_count;
    cache.nonprimes_cnt = np_count;
    cache.limit = limit;

    free(is_prime);
}

/* ---------- guarantee that we have enough data for index n ---------- */
static void ensure_capacity(idx_t n)
{
    /* Ensure at least n primes and n non‑primes are available */
    while (cache.primes_cnt < (size_t)n || cache.nonprimes_cnt < (size_t)n)
    {
        size_t new_limit = (cache.limit == 0) ? 2000 : cache.limit * 2;
        rebuild_sieve(new_limit);
    }

    /* Now fetch the n‑th prime and non‑prime values (1‑based) */
    uint32_t p_val = cache.primes[(size_t)(n - 1)];
    uint32_t np_val = cache.nonprimes[(size_t)(n - 1)];

    /* We need at least max(p_val, np_val) primes to index them. */
    size_t needed_primes = (p_val > np_val) ? p_val : np_val;
    while (cache.primes_cnt < needed_primes)
    {
        size_t new_limit = cache.limit * 2;
        rebuild_sieve(new_limit);
    }
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* outside offset – return 0 */
    ensure_capacity(n);

    uint32_t p_idx = cache.primes[(size_t)(n - 1)];      /* n‑th prime value */
    uint32_t np_idx = cache.nonprimes[(size_t)(n - 1)];  /* n‑th non‑prime value */

    /* Fetch prime at positions p_idx and np_idx */
    seq_t prime_p = (seq_t)cache.primes[p_idx - 1];
    seq_t prime_np = (seq_t)cache.primes[np_idx - 1];

    return prime_p > prime_np ? (prime_p - prime_np) : (prime_np - prime_p);
}
