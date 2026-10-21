/*
 * A053443  x^2 + y^2 does not take on all possible values mod n.
 * https://oeis.org/A053443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_bad_square, isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A053443: numbers n that are divisible by the square of a prime p,
    where p = 2 or p ≡ 3 (mod 4).  a(idx) returns the idx‑th such integer
    (1‑based index).  The routine builds a small prime table once and
    caches the sequence so repeated calls are O(1) after the first fill. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_bad_square.h"
#include "../utils/isqrt_u64.h"

/* Prime generation limit – large enough for the range of values needed
   (several tens of thousands of terms).  The prime sieve is done once. */
#define PRIME_SIEVE_LIMIT 200000ULL

/* Build a list of primes up to PRIME_SIEVE_LIMIT.
   The list is stored in a static array and initialized once. */
static void build_prime_list(const uint64_t **primes, size_t *primes_cnt)
{
    static uint64_t *prime_list = NULL;
    static size_t prime_cnt  = 0;

    if (prime_cnt != 0) {            /* already built */
        *primes = prime_list;
        *primes_cnt = prime_cnt;
        return;
    }

    /* Simple Eratosthenes sieve */
    size_t limit = PRIME_SIEVE_LIMIT;
    uint8_t *is_prime = (uint8_t *)malloc(limit + 1);
    if (!is_prime) abort();                 /* out of memory – hopeless */
    for (size_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* Count primes first to allocate exact space */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i) if (is_prime[i]) ++cnt;
    prime_list = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!prime_list) abort();

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i) if (is_prime[i]) prime_list[idx++] = i;
    free(is_prime);

    prime_cnt = cnt;
    *primes = prime_list;
    *primes_cnt = prime_cnt;
}

/* The required sequence function. */
seq_t a(idx_t n)
{
    /* Static cache for already computed terms. */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    static uint64_t last_checked = 0;      /* largest candidate examined */
    static const uint64_t *prime_list = NULL;
    static size_t prime_cnt = 0;

    /* Ensure the prime list is built once. */
    if (prime_cnt == 0) {
        build_prime_list(&prime_list, &prime_cnt);
    }

    if (n == 0) return (seq_t)0;   /* safeguard – the OEIS offset is 1 */

    /* Extend cache until we have at least n terms. */
    while (cache_len < (size_t)n) {
        ++last_checked;
        if (has_bad_square(last_checked, prime_list, prime_cnt)) {
            /* Grow cache if needed. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 128;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) abort();   /* out of memory */
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
