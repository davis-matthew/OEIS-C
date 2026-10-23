/*
 * A165350  Primes p such that floor((p^2-1)/4)+p is not prime.
 * https://oeis.org/A165350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Prime p such that floor((p^2-1)/4)+p is not prime (composite)
 * Range:     u8 n <= 1, u16 n <= 28, u32 n <= 95, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function iteratively builds the sequence by testing primes in order
   and caching those that satisfy the composite condition.  It works for
   any seq_t/idx_t pair provided the helper is_prime_big is available. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* ----  External utilities ---- */
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n); */

/* ----  Cache of qualifying primes ---- */
static seq_t *qual_cache = NULL; /* array of primes that satisfy the condition */
static size_t cache_len = 0;     /* number of cached terms */
static size_t cache_cap = 0;     /* current capacity of qual_cache */

/* ----  Cache of all primes (used for primality testing of candidates) ---- */
static seq_t *prime_list = NULL; /* array of all primes found so far */
static size_t primes_len = 0;    /* number of primes in prime_list */
static size_t primes_cap = 0;    /* capacity of prime_list */

/* ----  Grow a seq_t array to at least 'need' elements. ---- */
static void ensure_cap(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need)
        return;
    size_t new_cap = *cap ? *cap << 1 : 8;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (!tmp) abort();        /* simple error handling */
    *arr = tmp;
    *cap = new_cap;
}

/* ----  Integer square root (floor) for seq_t values. ---- */
static seq_t isqrt_seq(seq_t n)
{
    seq_t x = 0;
    while ((x + 1) * (x + 1) <= n)
        ++x;
    return x;
}

/* ----  Test whether n is prime using trial division by previously
   generated primes.  It falls back to is_prime_big for very large
   candidates that exceed the square of the largest cached prime. ---- */
static bool is_prime_candidate(seq_t n)
{
    if (n < 2)
        return false;
    seq_t root = isqrt_seq(n);
    for (size_t i = 0; i < primes_len; ++i) {
        seq_t p = prime_list[i];
        if (p > root)
            break;
        if (n % p == 0)
            return false;
    }
    /* All trials passed: determine primality using the general test. */
    return is_prime_big(n);
}

/* ----  Generate the next prime greater than the last returned by
   this generator, using the prime_list for test division. ---- */
static seq_t next_prime(void)
{
    static seq_t candidate = 2;
    static bool first = true;

    if (first) {
        first = false;
        /* Return 2 on the first call, then start from 3. */
        candidate = 3;
        /* Store 2 in prime_list. */
        ensure_cap(&prime_list, &primes_cap, primes_len + 1);
        prime_list[primes_len++] = 2;
        return 2;
    }

    while (!is_prime_candidate(candidate)) {
        candidate += (candidate == 2) ? 1 : 2; /* skip even numbers */
    }
    /* candidate is prime */
    seq_t prime = candidate;
    candidate += 2; /* next odd candidate */

    /* Cache this prime for future trial divisions */
    ensure_cap(&prime_list, &primes_cap, primes_len + 1);
    prime_list[primes_len++] = prime;

    return prime;
}

/* ----  Main sequence function a(idx_t n) ---- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* safety value for offsets < 1 */

    /* Expand cache until we have at least n terms. */
    while (cache_len < (size_t)n) {
        seq_t p = next_prime();

        /* Compute floor((p^2-1)/4)+p */
        seq_t val = p * p - 1;
        val = val / 4;
        val += p;

        if (!is_prime_big(val)) {      /* composite ⇒ qualifying prime */
            ensure_cap(&qual_cache, &cache_cap, cache_len + 1);
            qual_cache[cache_len++] = p;
        }
    }

    return qual_cache[n - 1];
}
