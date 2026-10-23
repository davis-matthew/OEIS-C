/*
 * A142776  Primes congruent to 49 mod 59.
 * https://oeis.org/A142776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 107, u32 n <= 39, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = the n‑th prime congruent to 49 modulo 59.
 * The function lazily generates terms and caches them for
 * subsequent calls, so the order of calls does not matter.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"  /* bool is_prime_big(seq_t n); */

/* arithmetic constants as seq_t values */
#define STEP  ((seq_t)59)

/* initial cache capacity */
#define INITIAL_CAP 32

/* static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* ensure the cache has room for at least one more element */
static void ensure_capacity(void)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : INITIAL_CAP;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0)  /* out of memory */
            exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)                /* offset is 1; undefined for 0 */
        return (seq_t)0;

    /* initialise cache on first use */
    if (!cache) {
        cache = (seq_t *)malloc(INITIAL_CAP * sizeof(seq_t));
        if (!cache)
            exit(EXIT_FAILURE);
        cache_cap = INITIAL_CAP;
    }

    /* start from the first candidate or the next after the last stored prime */
    seq_t cand;
    if (cache_len == 0) {
        cand = (seq_t)59 * 2 + (seq_t)49;          /* 167 is the first term */
    } else {
        cand = cache[cache_len - 1] + STEP;
    }

    /* generate primes until we have at least n of them */
    while (cache_len < (size_t)n) {
        if (is_prime_big(cand)) {
            ensure_capacity();
            cache[cache_len++] = cand;
        }
        cand += STEP;
    }

    return cache[n - 1];
}
