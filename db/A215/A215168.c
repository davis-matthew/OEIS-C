/*
 * A215168  Primes congruent to {2, 5} mod 13.
 * https://oeis.org/A215168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes congruent to 2 or 5 modulo 13
 * Range:     u8 n <= 10, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *cache = NULL;       /* cached terms */
static size_t cache_len = 0;      /* number of cached terms */
static size_t cache_cap = 0;      /* allocated capacity */
static uint64_t last_checked = 2; /* next candidate to try */

/* ensure the cache can hold at least 'need' elements */
static void ensure_cache_cap(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < need)
            new_cap <<= 1;
        seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_arr)
            exit(1);
        cache = new_arr;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)             /* undefined for n==0, but return 0 */
        return 0;

    /* return cached value if already computed */
    if ((size_t)n <= cache_len)
        return cache[(size_t)n - 1];

    /* extend the cache until we have n terms */
    uint64_t cand = last_checked;
    while (cache_len < (size_t)n) {
        if (cand == 2) {
            /* 2 (mod 13) is a valid residue, and 2 is prime */
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = (seq_t)cand;
        } else {
            /* test only if candidate matches the required residues */
            if ((cand & 1) && /* skip even numbers */
                (cand % 13ULL == 2ULL || cand % 13ULL == 5ULL) &&
                is_prime(cand)) {
                ensure_cache_cap(cache_len + 1);
                cache[cache_len++] = (seq_t)cand;
            }
        }
        ++cand;
    }

    last_checked = cand;               /* remember where we left off */
    return cache[(size_t)n - 1];
}
