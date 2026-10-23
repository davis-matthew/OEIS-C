/*
 * A140732  Primes congruent to 71 or 139 mod 210.
 * https://oeis.org/A140732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 271, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A140732: Primes congruent to 71 or 139 (mod 210) */
/* The nth term is the nth prime p such that p ≡ 71 or 139 (mod 210). */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/is_prime.h"   /* bool is_prime(idx_t x); */

static const unsigned int residues[2] = {71, 139};

/* Cache of already found primes.  We grow it in powers of two. */
static seq_t *cache = NULL;
static size_t cache_size = 0; /* number of primes stored */
static size_t cache_cap  = 0; /* allocated capacity */

/* Ensure the cache can hold at least `needed` elements. */
static void
ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Store a newly found prime in the cache. */
static void
add_prime(seq_t val)
{
    ensure_cache_cap(cache_size + 1);
    cache[cache_size++] = val;
}

/* State for incremental search through the progression 210*k + r. */
static idx_t last_k   = 0; /* current k in 210*k + r */
static int   last_idx = 0; /* 0 -> 71, 1 -> 139 */

seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1; return 0 for out‑of‑range indices */
        return (seq_t)0;

    /* Return cached value if already computed. */
    if (n <= (idx_t)cache_size)
        return cache[n - 1];

    /* Find further primes until we have at least n of them. */
    while (cache_size < n) {
        /* candidate = 210*k + r */
        uint64_t cand64 = 210ULL * (uint64_t)last_k + residues[last_idx];
        /* Test for primality */
        if (is_prime((idx_t)cand64))
            add_prime((seq_t)cand64);

        /* Advance to the next residue; after both, increment k. */
        ++last_idx;
        if (last_idx >= 2) {
            last_idx = 0;
            ++last_k;
        }
    }

    return cache[n - 1];
}
