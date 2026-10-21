/*
 * A092581  a(n) is the least prime such that a(n-1) is a quadratic
 *          non-residue of a(n).
 * https://oeis.org/A092581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow, next_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A092581
   a(1) = 2
   a(n) is the least prime p > a(n-1) such that a(n‑1) is a quadratic non‑residue modulo p
   (OEIS offset 1) */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/next_prime.h"
#include "../utils/mod_pow.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;   /* cached terms a(1)... */
static size_t cache_size = 0; /* number of stored terms */
static size_t cache_cap  = 0; /* allocated capacity */

static void ensure_capacity(idx_t n)
{
    if (cache_cap >= (size_t)n) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < (size_t)n) new_cap = n;
    seq_t *new_cache = seq_realloc(cache, cache_cap, new_cap);
    cache = new_cache;
    cache_cap = new_cap;
}

/* Compute the next term assuming cache[cache_size-1] holds a(n-1). */
static void compute_next(void)
{
    seq_t prev = cache[cache_size - 1];
    /* Start with the smallest prime strictly greater than prev. */
    uint64_t pv = (uint64_t)prev;
    uint64_t p = next_prime(pv);

    for (;;)
    {
        /* Legendre symbol test: a^( (p-1)/2 ) mod p == p-1  =>  a is a non‑residue mod p */
        uint64_t exp = (p - 1) >> 1;
        seq_t res = mod_pow(prev, exp, (seq_t)p);
        if (res == (seq_t)(p - 1))
            break;                     /* found the least prime with the property */
        p = next_prime(p);             /* next prime > p */
    }
    cache[cache_size] = (seq_t)p;       /* store a(n) */
}

static void ensure_computed(idx_t n)
{
    if (cache_size >= n) return;
    ensure_capacity(n);
    for (; cache_size < n; ++cache_size)
    {
        if (cache_size == 0)          /* initialize a(1) */
            cache[0] = (seq_t)2;
        else
            compute_next();            /* compute a(cache_size+1) */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* undefined for n ≤ 0 */
    ensure_computed(n);
    return cache[n - 1];
}
