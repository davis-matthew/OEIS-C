/*
 * A042988  Primes not congruent to -1 (mod 7).
 * https://oeis.org/A042988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the (n+1)-th prime p with p % 7 != 6 (primes not
 *            congruent to -1 mod 7)
 * Range:     u8 n <= 44, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t next_candidate = 2;
static bool initialized = false;

static void grow_cache(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < needed)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (!initialized) {
        cache = NULL;
        cache_cap = 0;
        cache_len = 0;
        next_candidate = 2;
        initialized = true;
    }

    while (cache_len <= n) {
        idx_t cand = next_candidate;
        next_candidate = cand + 1;
        if (is_prime(cand) && (cand % 7u) != 6u) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)cand;
        }
    }
    return cache[n];
}
