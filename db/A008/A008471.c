/*
 * A008471  Exactly 3 out of 10m+1, 10m+3, 10m+7, 10m+9 are primes.
 * https://oeis.org/A008471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th positive integer m such that exactly three of
 *            the four numbers 10m+1, 10m+3, 10m+7, 10m+9 are prime
 * Range:     u8 n <= 23, u16 n <= 1529, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Cached values and state for O(1) subsequent calls. */
static seq_t *cache = NULL;  /* array of found m's */
static idx_t  cache_count = 0;    /* how many m's stored */
static idx_t  cache_cap    = 0;    /* allocated capacity */
static idx_t  last_checked = 0;   /* largest m tested so far */

/* Ensure that the cache has room for at least `need` elements. */
static void ensure_cache_capacity(idx_t need) {
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < (size_t)need) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) exit(1);           /* abort on allocation failure */
    cache = new_cache;
    cache_cap = new_cap;
}

/* Count how many of the four values are prime; return the count. */
static int prime_bits(idx_t m) {
    int cnt = 0;
    if (is_prime(10 * m + 1ULL)) ++cnt;
    if (is_prime(10 * m + 3ULL)) ++cnt;
    if (is_prime(10 * m + 7ULL)) ++cnt;
    if (is_prime(10 * m + 9ULL)) ++cnt;
    return cnt;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for offset 1, but harmless */

    /* Grow the cache until it contains at least n elements. */
    while (cache_count < n) {
        ensure_cache_capacity(cache_count + 1);
        ++last_checked;                      /* test next m value          */
        if (prime_bits(last_checked) == 3) {  /* record if exactly 3 primes */
            cache[cache_count++] = (seq_t)last_checked;
        }
    }
    return cache[n - 1];
}
