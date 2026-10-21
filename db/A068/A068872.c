/*
 * A068872  Composite numbers with all divisors congruent to 1 mod 10.
 * https://oeis.org/A068872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth composite number whose every divisor is ≡ 1 (mod
 *            10)
 * Range:     u8 n <= 1, u16 n <= 527, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <math.h>

static uint64_t *primes = NULL;
static size_t primes_len = 0;
static size_t primes_cap = 0;

/* Ensure that the list of primes contains all primes up to at least 'limit'. */
static void ensure_primes_to(uint64_t limit)
{
    if (primes_len > 0 && primes[primes_len - 1] >= limit)
        return;

    if (primes_cap == 0) {
        primes_cap = 32;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
    }

    uint64_t start = (primes_len == 0) ? 2 : primes[primes_len - 1] + 1;
    for (uint64_t cand = start; cand <= limit; ++cand) {
        bool isprime = true;
        uint64_t r = (uint64_t) sqrt((double) cand);
        for (size_t i = 0; i < primes_len && primes[i] <= r; ++i) {
            if (cand % primes[i] == 0) {
                isprime = false;
                break;
            }
        }
        if (isprime) {
            if (primes_len == primes_cap) {
                primes_cap *= 2;
                primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
            }
            primes[primes_len++] = cand;
        }
    }
}

/* Test whether x is a composite number all of whose prime factors are ≡ 1 (mod 10). */
static bool is_allowed_composite_uint64(uint64_t x)
{
    if (x < 2)
        return false;

    uint64_t sqrtx = (uint64_t) sqrt((double) x);
    ensure_primes_to(sqrtx);

    uint64_t temp = x;
    bool composite = false;
    for (size_t i = 0; i < primes_len && primes[i] <= sqrtx; ++i) {
        uint64_t p = primes[i];
        if (temp % p == 0) {
            composite = true;
            if (p % 10 != 1)
                return false;
            while (temp % p == 0)
                temp /= p;
        }
    }

    /* After division by all primes ≤ sqrt(x). */
    if (!composite && temp == x)   /* x is prime */
        return false;

    if (temp > 1) {                 /* remaining prime factor > sqrt(x) */
        if (temp % 10 != 1)
            return false;
        composite = true;
    }

    return composite;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    while (cache_len < n) {
        uint64_t cur;
        if (cache_len == 0)
            cur = 121;                      /* first term of the sequence */
        else
            cur = (uint64_t) cache[cache_len - 1] + 1;

        while (!is_allowed_composite_uint64(cur))
            ++cur;

        if (cache_len == cache_cap) {
            cache_cap = cache_cap ? cache_cap * 2 : 16;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len++] = (seq_t) cur;
    }
    return cache[n - 1];
}
