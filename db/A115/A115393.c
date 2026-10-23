/*
 * A115393  Numbers k such that k, k-1 and k-2 are semiprimes.
 * https://oeis.org/A115393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 280, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A115393: numbers k such that k, k-1 and k-2 are semiprimes
   (computed by on‑demand search with memoisation). */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- integer square root for uint64_t ---------- */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = n;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

/* ---------- prime sieve helpers ---------- */
static uint64_t prime_limit = 0;
static size_t prime_count = 0;
static size_t prime_capacity = 0;
static uint32_t *primes = NULL;

static void ensure_prime_limit(uint64_t limit)
{
    if (limit <= prime_limit) return;

    uint8_t *isprime = (uint8_t*)calloc((size_t)limit + 1, 1);
    if (!isprime) abort();
    for (uint64_t i = 2; i <= limit; ++i) isprime[i] = 1;

    size_t count = 0;
    uint64_t sqrt_limit = isqrt_u64(limit);
    for (uint64_t i = 2; i <= limit; ++i) {
        if (isprime[i]) {
            if (count >= prime_capacity) {
                prime_capacity = prime_capacity ? prime_capacity * 2 : 1024;
                primes = (uint32_t*)realloc(primes, prime_capacity * sizeof(uint32_t));
                if (!primes) abort();
            }
            primes[count++] = (uint32_t)i;
            if (i <= sqrt_limit) {
                for (uint64_t j = i * i; j <= limit; j += i)
                    isprime[j] = 0;
            }
        }
    }
    free(isprime);
    prime_limit = limit;
    prime_count = count;
}

/* ---------- primality test using the prime list ---------- */
static inline bool is_prime_small(uint64_t n)
{
    if (n < 2) return false;
    ensure_prime_limit(isqrt_u64(n));
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* ---------- semiprime test ---------- */
static inline bool is_semiprime_u64(uint64_t x)
{
    if (x < 4) return false;                /* 2*2 is the smallest semiprime */
    ensure_prime_limit(isqrt_u64(x));
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > x) break;
        if (x % p == 0) {
            uint64_t q = x / p;
            return is_prime_small(q);
        }
    }
    return false;                           /* x is prime or 1 */
}

/* ---------- sequence cache ---------- */
static seq_t *terms_cache = NULL;
static size_t used_terms = 0;
static size_t cache_cap = 0;
static uint64_t last_found = 0;

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* offset is 1; behaviour for n<1 is irrelevant */

    /* initialise cache array lazily */
    if (cache_cap == 0) {
        cache_cap = 64;
        terms_cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!terms_cache) abort();
    }

    /* extend cache until it contains the n‑th term */
    while (used_terms < (size_t)n) {
        uint64_t k = last_found ? last_found + 1 : 3;
        for (;;) {
            if (is_semiprime_u64(k - 2) &&
                is_semiprime_u64(k - 1) &&
                is_semiprime_u64(k)) {
                /* store term, expanding cache if needed */
                if (used_terms == cache_cap) {
                    cache_cap *= 2;
                    terms_cache = (seq_t*)realloc(terms_cache, cache_cap * sizeof(seq_t));
                    if (!terms_cache) abort();
                }
                terms_cache[used_terms++] = (seq_t)k;
                last_found = k;
                break;
            }
            ++k;
        }
    }

    return terms_cache[(size_t)n - 1];
}
