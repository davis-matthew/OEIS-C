/*
 * A141796  Composites of the form n/(prime(n-1) - prime(n-2)).
 * https://oeis.org/A141796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n/(prime(n-1) - prime(n-2)) , taking only composite
 *            values
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A141796  */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime.h"

/* static cache for the sequence values */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;

/* static array for sieving primes up to required index */
static uint64_t *primes   = NULL;
static size_t primes_cap  = 0;
static size_t primes_len  = 0;
static uint64_t last_prime = 1;

/* next n to test for the sequence */
static idx_t current_n = 3;

/* make sure we have at least `idx` primes stored (indices 1..idx) */
static void ensure_primes_up_to(idx_t idx)
{
    if (primes_len >= (size_t)idx)
        return;

    if (primes_len == 0) {
        size_t init = 16;
        primes = (uint64_t *)malloc(init * sizeof(uint64_t));
        if (!primes) abort();
        primes_cap = init;
        /* first prime 2 */
        primes[0] = 2;
        primes_len = 1;
        last_prime = 2;
    }

    if (primes_len + 1 > primes_cap) {
        size_t newcap = primes_cap * 2;
        uint64_t *tmp = (uint64_t *)realloc(primes, newcap * sizeof(uint64_t));
        if (!tmp) abort();
        primes = tmp;
        primes_cap = newcap;
    }

    for (; primes_len < (size_t)idx; ++primes_len) {
        uint64_t candidate = last_prime + 1;
        for (;;) {
            if (is_prime(candidate)) {
                last_prime = candidate;
                primes[primes_len] = candidate;
                break;
            }
            ++candidate;
        }
    }
}

/* make sure the cache can hold at least `needed` values */
static void ensure_cache_capacity(size_t needed)
{
    if (cache_len >= needed)
        return;
    if (cache_len + needed > cache_cap) {
        size_t newcap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = newcap;
    }
}

/* generate sequence values until we have at least `target_index` entries */
static void generate_until(idx_t target_index)
{
    while (cache_len < target_index) {
        if (current_n < 3) {      /* safety check – should not happen */
            ++current_n;
            continue;
        }

        /* need primes up to index (current_n-1) */
        ensure_primes_up_to((size_t)(current_n - 1));

        uint64_t prime_prev  = primes[current_n - 2]; /* prime(n-1) */
        uint64_t prime_prev2 = primes[current_n - 3]; /* prime(n-2) */
        uint64_t diff = prime_prev - prime_prev2;

        if (diff != 0 && current_n % diff == 0) {
            idx_t val = current_n / (idx_t)diff;
            if (val > 1 && !is_prime(val)) {
                ensure_cache_capacity(cache_len + 1);
                cache[cache_len++] = (seq_t)val;
            }
        }
        ++current_n;
    }
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* offset starts at 1 */
    if (cache_len < n)
        generate_until(n);
    return cache[n - 1];
}
