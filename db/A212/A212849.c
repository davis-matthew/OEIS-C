/*
 * A212849  Numbers whose sum of prime factors (counted with multiplicity) is
 *          a triangular number.
 * https://oeis.org/A212849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 2445, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   factor_u64, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: a(n) is the n-th positive integer whose sum of
   prime factors (counted with multiplicity) is a triangular number. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_realloc.h"

/* 64‑bit integer square root (integer only, no floating point). */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t res = 0;
    uint64_t bit = 1ULL << 62;          /* largest power of 4 <= n */
    while (bit > n) bit >>= 2;
    while (bit)
    {
        if (n >= res + bit)
        {
            n -= res + bit;
            res = (res >> 1) + bit;
        }
        else
        {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

/* Check whether x is a triangular number (x = k*(k+1)/2 for some k).
   Returns true for 0 (t_0). */
static bool triangular_uint64(uint64_t x)
{
    if (x == 0) return true;
    __int128 d = (__int128)8 * x + 1;          /* 8*x + 1 must be a square */
    if (d > UINT64_MAX) return false;          /* overflow -> cannot be square */
    uint64_t dd = (uint64_t)d;
    uint64_t s = isqrt_u64(dd);
    return s * s == dd;
}

/* Static cache for the computed sequence. */
static seq_t *cache   = NULL;
static size_t cache_cap = 0;    /* allocated capacity */
static size_t cache_len = 0;    /* number of entries stored */

/* Grow the cache to contain at least 'need' slots. */
static void grow_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 4;
    while (new_cap < need) new_cap <<= 1;
    cache = seq_realloc(cache, cache_cap, new_cap);
    cache_cap = new_cap;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, harmless */
    /* Return cached value if already available. */
    if ((size_t)n <= cache_len) return cache[n-1];

    /* Search numbers starting from the first integer after the last cached one. */
    uint64_t cand = cache_len == 0 ? 1 : (uint64_t)cache[cache_len-1] + 1;

    while (cache_len < (size_t)n)
    {
        /* 1. Compute sum of prime factors with multiplicity. */
        uint64_t primes[64];
        uint64_t exps[64];
        size_t cnt = 0;
        factor_u64(cand, primes, exps, &cnt);

        uint64_t sop = 0;          /* sum of prime factors (repeated) */
        for (size_t i = 0; i < cnt; ++i)
            sop += primes[i] * exps[i];

        /* 2. Test whether sop is triangular. */
        if (triangular_uint64(sop))
        {
            /* 3. Cache the candidate. */
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)cand;
        }
        ++cand;
    }

    return cache[n-1];
}
