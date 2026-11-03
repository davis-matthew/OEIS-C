/*
 * A071593  Number of 1's in binary representation of n equals tau(n), the
 *          number of divisors of n.
 * https://oeis.org/A071593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = nth positive integer k such that popcount(k) = τ(k)
 * Range:     u8 n <= 32, u16 n <= 3715, u32 n <= 51000, u64 n <= 51000,
 *            u128 n <= 51000, big any n
 * Validated: standard, pass, 72 terms through n = 9305 (2026-11-03)
 * Helpers:   mem, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/num_divisors.h"
#include "../utils/mem.h"

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
        uint64_t  div = num_divisors(last_candidate);

        if (pop == div) {                                /* satisfies condition? */
            if (cache_len == cache_cap) {                /* grow the cache  */
                size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
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
