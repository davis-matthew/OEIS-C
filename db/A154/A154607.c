/*
 * A154607  Numbers n such that 11*n + 4 is prime.
 * https://oeis.org/A154607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th positive integer m such that 11*m + 4 is prime.
 *            * Search is performed lazily, caching found values so any order
 *            of calls * is fast. Primality test uses the provided
 *            is_prime_big, which works for * all seq_t sizes (bytes, words,
 *            or arbitrary precision)
 * Range:     u8 n <= 4, u16 n <= 506, u32 n <= 507, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Prime test for arbitrary precision or small seq_t values. */
#include "../utils/is_prime_big.h"

static seq_t *cache       = NULL;   /* cached a(n) values */
static size_t cache_size  = 0;      /* number of stored values   */
static size_t cache_cap   = 0;      /* allocated capacity        */
static idx_t next_candidate = 1;    /* next integer to test       */

/* Ensure that `cache` contains at least `target` entries. */
static void ensure_cache(size_t target)
{
    if (target <= cache_size)
        return;

    if (!cache) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            exit(1);
    }

    /* Expand cache until we have `target` entries. */
    while (cache_size < target) {
        idx_t val = 11 * next_candidate + 4;          /* 11*m+4   */
        seq_t p = (seq_t)val;                         /* may be 64‑bit   */
        if (is_prime_big(p)) {                        /* prime test     */
            if (cache_size == cache_cap) {
                cache_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp)
                    exit(1);
                cache = tmp;
            }
            cache[cache_size++] = (seq_t)next_candidate;
        }
        next_candidate++;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;              /* undefined index – safe fallback */

    ensure_cache((size_t)n);
    return cache[n - 1];
}
