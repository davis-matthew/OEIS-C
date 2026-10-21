/*
 * A235693  Semiprimes which have one or more occurrences of exactly five
 *          different digits.
 * https://oeis.org/A235693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4077, u32 n <= 6240, u64 n <= 6240,
 *            u128 n <= 6240, big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A235693.
   Sequence: Semiprimes which have exactly five distinct decimal digits.
   a(1) = 10237, a(2) = 10238, … */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_counts.h"

/* --------------------------------------------------------------------- */
/*  Prime generation utilities                                            */
/* --------------------------------------------------------------------- */

static uint64_t *primes = NULL;
static size_t primes_len = 0, primes_cap = 0;
static uint64_t primes_next = 2;

static void ensure_primes_up_to(uint64_t limit)
{
    if (limit < primes_next - 1)
        return;

    for (uint64_t cand = primes_next; cand <= limit; ++cand) {
        bool is_prime = true;
        for (size_t i = 0; i < primes_len; ++i) {
            uint64_t p = primes[i];
            if (p * p > cand)
                break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (primes_len == primes_cap) {
                primes_cap = primes_cap ? primes_cap * 2 : 8;
                primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
            }
            primes[primes_len++] = cand;
        }
    }
    primes_next = limit + 1;
}

/* --------------------------------------------------------------------- */
/*  Helpers for the sequence                                            */
/* --------------------------------------------------------------------- */

static size_t count_distinct_digits(seq_t x)
{
    unsigned char cnt[10];
    digit_counts(x, cnt);
    size_t distinct = 0;
    for (int d = 0; d < 10; ++d)
        if (cnt[d] > 0)
            ++distinct;
    return distinct;
}

static bool is_semiprime(seq_t x)
{
    if (x < 4)
        return false;

    uint64_t n = (uint64_t)x;
    uint64_t temp = n;
    size_t factors = 0;

    /* Ensure we have primes up to sqrt(n) */
    ensure_primes_up_to((uint64_t)/*sqrt approximation*/(int)((double)n * 0.5 + 1));

    for (size_t i = 0; i < primes_len && primes[i] * primes[i] <= temp; ++i) {
        uint64_t p = primes[i];
        while (temp % p == 0) {
            temp /= p;
            ++factors;
            if (factors > 2)
                return false;
        }
    }
    if (temp > 1)
        ++factors;

    return factors == 2;
}

/* --------------------------------------------------------------------- */
/*  Main sequence function                                               */
/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* Cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;

    /* Next integer to test (semiprimes are >= 4) */
    static seq_t next_candidate = 4;

    while (cache_len < n) {
        ++next_candidate;
        if (is_semiprime(next_candidate) && count_distinct_digits(next_candidate) == 5) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 8;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = next_candidate;
        }
    }

    return cache[n - 1];
}
