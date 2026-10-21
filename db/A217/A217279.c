/*
 * A217279  Numbers of the form n^2 + 1 without prime divisors of the form a^2
 *          + 1.
 * https://oeis.org/A217279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 12, u32 n <= 4794, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of A217279 (Numbers of the form n^2 + 1 without prime divisors of the form a^2 + 1). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Helper functions provided by the build system */
#include "../utils/factor_u64.h"
#include "../utils/floor_sqrt.h"

static seq_t *cache = NULL;          /* array of found terms */
static size_t cache_len = 0;         /* number of terms stored */
static size_t cache_cap = 0;         /* allocated capacity */
static uint64_t next_t = 1;          /* next t to test */

/* Ensure the cache has room for at least one more element */
static void cache_grow(void)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 128;
        seq_t *newp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!newp) abort();
        if (cache) {
            memcpy(newp, cache, cache_len * sizeof(seq_t));
            free(cache);
        }
        cache = newp;
        cache_cap = new_cap;
    }
}

/* Test whether a prime number p is of the form a^2 + 1 */
static bool prime_of_square_plus_one(uint64_t p)
{
    /* For primes p > 2, p must be odd; p-1 is even. */
    seq_t root = floor_sqrt((seq_t)(p - 1));
    seq_t sq = root * root;
    return sq == (seq_t)(p - 1);
}

seq_t a(idx_t n)
{
    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        uint64_t t = next_t++;
        seq_t val = ((seq_t)t) * ((seq_t)t) + 1;  /* n^2 + 1 */

        /* Convert to 64‑bit for factorisation (works for all values in test range) */
        uint64_t val64 = (uint64_t)val;
        /* Factor val using the helper routine */
        uint64_t primes[32], exps[32];
        size_t cnt = 0;
        factor_u64(val64, primes, exps, &cnt);

        /* Check each prime factor */
        bool bad = false;
        for (size_t i = 0; i < cnt && !bad; ++i) {
            uint64_t p = primes[i];
            if (prime_of_square_plus_one(p))
                bad = true;
        }

        if (!bad) {
            cache_grow();
            cache[cache_len++] = val;
        }
    }

    return cache[(size_t)n - 1];
}
