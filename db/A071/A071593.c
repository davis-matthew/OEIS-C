/*
 * A071593  Number of 1's in binary representation of n equals tau(n), the
 *          number of divisors of n.
 * https://oeis.org/A071593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = nth positive integer k such that popcount(k) = τ(k)
 * Range:     u8 n <= 32, u16 n <= 3715, u32 n <= 9305, u64 n <= 9305,
 *            u128 n <= 9305, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/divisor_count_u64.h"

/* Cache of the found terms. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Highest integer examined so far. */
static uint64_t last_candidate = 0;

static void
ensure_cache(idx_t n)
{
    while (cache_len < n) {
        last_candidate++;                                /* next candidate  */
        unsigned pop = __builtin_popcountll(last_candidate);
        uint64_t  div = divisor_count_u64(last_candidate);

        if (pop == div) {                                /* satisfies condition? */
            if (cache_len == cache_cap) {                /* grow the cache  */
                size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                seq_t *tmp = (seq_t *)realloc(cache,
                                               new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache     = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len] = (seq_t)last_candidate;    /* store term      */
            cache_len++;
        }
    }
}

/* Public entry point: the nth term (1‑based) of A071593. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for < 1, harmless */
    ensure_cache(n);                      /* compute terms up to n          */
    return cache[n - 1];                   /* zero‑based indexing            */
}
