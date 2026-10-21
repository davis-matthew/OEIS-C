/*
 * A078575  Sum of divisors of the average of n-th twin prime pair.
 * https://oeis.org/A078575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000203(A014574(n)) (sum of divisors of the average of
 *            the n‑th twin prime pair)
 * Range:     u8 n <= 9, u16 n <= 350, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* is_prime(idx_t) */
#include "../utils/sigma_of.h"  /* sigma_of(idx_t) */

static uint64_t *cache_avg = NULL;   /* cached averages p+1 of twin prime pairs */
static size_t   cache_len  = 0;       /* number of cached averages */
static size_t   cache_cap  = 0;       /* capacity of cache_avg array */
static uint64_t next_candidate = 3;   /* next odd number to test as lower twin */

/* Grow the cache array to at least min_cap elements */
static void grow_cache(size_t min_cap)
{
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < min_cap) new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(cache_avg, new_cap * sizeof(uint64_t));
    if (!tmp) abort();                /* allocation failed */
    cache_avg = tmp;
    cache_cap = new_cap;
}

/* OEIS sequence A078575: sum of divisors of the average of the n‑th twin prime pair */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for 0, safe default */

    /* Ensure cache contains the first n averages */
    while (cache_len < (size_t)n) {
        uint64_t p = next_candidate;
        for (;;) {                  /* find next twin prime pair (p, p+2) */
            if (is_prime(p) && is_prime(p + 2)) {
                if (cache_len + 1 > cache_cap) grow_cache(cache_len + 1);
                cache_avg[cache_len++] = p + 1;     /* average of the pair */
                next_candidate = p + 2;             /* start after the second prime */
                break;
            }
            p += 2;                  /* skip even candidates */
            if (p < next_candidate) abort();     /* protect against overflow */
        }
    }

    /* Have the average of the n‑th twin prime pair in cache_avg[n‑1] */
    seq_t avg = (seq_t)cache_avg[(size_t)n - 1];
    return sigma_of((idx_t)avg);
}
