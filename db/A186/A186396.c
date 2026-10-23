/*
 * A186396  Numbers k such that there are 4 primes between 100*k and 100*k +
 *          99.
 * https://oeis.org/A186396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A186396: a(n) is the nth integer k such that there are
   exactly 4 primes in the interval [100*k, 100*k+99].
   The function computes terms on demand, caching previously found
   values so that calls in any order are O(1) after the initial
   sequential generation. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"      /* bool is_prime(seq_t n); */

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_k = 1;

static void expand_cache(void)
{
    if (cache_cap == 0) {
        cache_cap = 128;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    } else if (cache_len >= cache_cap) {
        cache_cap *= 2;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n < 1 */

    /* Generate terms until we have at least n entries */
    while (cache_len < (size_t)n) {
        uint64_t low  = 100 * next_k;
        uint64_t high = low + 99;
        int prime_cnt = 0;

        for (uint64_t x = low; x <= high; ++x) {
            if (is_prime(x)) prime_cnt++;
        }

        if (prime_cnt == 4) {
            expand_cache();
            cache[cache_len++] = (seq_t)next_k;
        }
        next_k++;
    }

    return cache[(size_t)n - 1];
}
