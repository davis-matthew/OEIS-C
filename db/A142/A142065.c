/*
 * A142065  Primes congruent to 28 mod 33.
 * https://oeis.org/A142065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 284, u32 n <= 289, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes p ≡ 28 (mod 33) by incremental search. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* --------------------------------------------------------------------- */
/*  Static cache for the sequence values.                                 */
/* --------------------------------------------------------------------- */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;
static idx_t  next_k     = 1;   /* next value of k in 33*k+28 */
static bool   initialized = false;

/* --------------------------------------------------------------------- */
static void init_cache(void)
{
    if (initialized) return;
    cache = seq_alloc(16);
    cache_cap = 16;
    cache_len = 0;
    next_k = 1;
    initialized = true;
}

/* --------------------------------------------------------------------- */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
    cache = tmp;
    cache_cap = new_cap;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* zero for indices before the offset */

    init_cache();

    while (cache_len < (size_t)n) {
        /* candidate of the form 33*k + 28 */
        uint64_t cand_uint64 = 33 * (uint64_t)next_k + 28ULL;
        seq_t cand = (seq_t)cand_uint64;

        if (is_prime_big(cand)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = cand;
        }
        ++next_k;
    }

    return cache[n - 1];
}
