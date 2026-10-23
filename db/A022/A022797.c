/*
 * A022797  a(n) = n-th prime + n-th nonprime.
 * https://oeis.org/A022797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = A000040(n) + A018252(n)
 * Range:     u8 n <= 44, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime.h"

static uint64_t *prime_cache   = NULL;
static size_t   prime_count   = 0;
static size_t   prime_cap     = 0;

static uint64_t *nonprime_cache = NULL;
static size_t   nonprime_count = 0;
static size_t   nonprime_cap   = 0;

static void ensure_primes(idx_t n)
{
    if (prime_count >= n)
        return;
    if (prime_cap < (size_t)n) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
        while (new_cap < (size_t)n)
            new_cap *= 2;
        uint64_t *tmp = (uint64_t *)realloc((void *)prime_cache, new_cap * sizeof(uint64_t));
        if (!tmp)
            return;
        prime_cache = tmp;
        prime_cap   = new_cap;
    }
    uint64_t cand = (prime_count == 0) ? 2ULL : prime_cache[prime_count - 1] + 1ULL;
    while (prime_count < (size_t)n) {
        if (is_prime(cand))
            prime_cache[prime_count++] = cand;
        ++cand;
    }
}

static void ensure_nonprimes(idx_t n)
{
    if (nonprime_count >= n)
        return;
    if (nonprime_cap < (size_t)n) {
        size_t new_cap = nonprime_cap ? nonprime_cap * 2 : 16;
        while (new_cap < (size_t)n)
            new_cap *= 2;
        uint64_t *tmp = (uint64_t *)realloc((void *)nonprime_cache, new_cap * sizeof(uint64_t));
        if (!tmp)
            return;
        nonprime_cache = tmp;
        nonprime_cap   = new_cap;
    }
    uint64_t cand = (nonprime_count == 0) ? 1ULL : nonprime_cache[nonprime_count - 1] + 1ULL;
    while (nonprime_count < (size_t)n) {
        if (cand == 1ULL || !is_prime(cand))
            nonprime_cache[nonprime_count++] = cand;
        ++cand;
    }
}

extern seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    ensure_primes(n);
    ensure_nonprimes(n);

    return (seq_t)prime_cache[n - 1] + (seq_t)nonprime_cache[n - 1];
}
