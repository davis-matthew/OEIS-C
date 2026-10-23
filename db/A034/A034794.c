/*
 * A034794  a(n) is the least prime p > a(n-1) such that a(n-1) is a quadratic
 *          residue mod p.
 * https://oeis.org/A034794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is defined recursively as follows: a(1)=2, and for n>1, a(n) is the least prime p > a(n-1) such that a(n-1) is a quadratic residue modulo p. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/mod_pow.h"

static seq_t *cache = NULL;          /* cache[1 .. cached_n] holds a(i) as seq_t */
static uint64_t *int_cache = NULL;   /* cache of the same values as uint64_t for primality work */
static idx_t cached_n = 0;           /* largest index for which cache is set */
static size_t cache_cap = 0;         /* number of allocated slots (index 0 unused) */

static void ensure_capacity(idx_t up_to)
{
    if (cache == NULL) {
        cache_cap = 8;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();
    }
    if (int_cache == NULL) {
        int_cache = (uint64_t*)malloc(cache_cap * sizeof(uint64_t));
        if (int_cache == NULL) abort();
    }

    /* need slots up to 'up_to' inclusive (index 0 unused) */
    size_t needed = (size_t)up_to + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cache_cap = new_cap;
        uint64_t *tmp2 = (uint64_t*)realloc(int_cache, new_cap * sizeof(uint64_t));
        if (tmp2 == NULL) abort();
        int_cache = tmp2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety for indices below offset */

    if (cached_n == 0) {           /* initialise first term */
        ensure_capacity(1);
        cache[1]   = (seq_t)2;
        int_cache[1] = 2ULL;
        cached_n = 1;
    }

    if (n > cached_n) {
        ensure_capacity(n);
        for (idx_t i = cached_n + 1; i <= n; ++i) {
            uint64_t prev_val = int_cache[i - 1];          /* previous prime */
            uint64_t cand = next_prime(prev_val + 1);      /* start searching */
            while (true) {
                /* Legendre symbol: compute prev_val^((cand-1)/2) mod cand */
                uint64_t exp = (cand - 1ULL) >> 1;          /* (p-1)/2 */
                seq_t  r = mod_pow((seq_t)prev_val, exp, (seq_t)cand);
                if (r == (seq_t)1) break;                  /* quadratic residue */
                cand = next_prime(cand + 1);
            }
            cache[i] = (seq_t)cand;
            int_cache[i] = cand;
        }
        cached_n = n;
    }

    return cache[n];
}
