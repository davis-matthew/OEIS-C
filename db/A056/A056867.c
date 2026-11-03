/*
 * A056867  Nilpotent numbers: n such that every group of order n is
 *          nilpotent.
 * https://oeis.org/A056867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 109, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9926 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Nilpotent numbers: n is nilpotent iff for every pair of distinct primes
   p and q dividing n, we have p^k ≢ 1 (mod q) for all 1 ≤ k ≤ exp_p(n). */

/* The code searches for nilpotent numbers, caching results so that
   any index can be answered in any order. No external helpers are used. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Check whether a 64‑bit integer m is nilpotent. */
static bool is_nilpotent(uint64_t m)
{
    if (m <= 1)
        return true;

    /* Factorise m.  The number of distinct primes is small for the ranges
       we need (≤ ~ 15 for m < 10^6).  We use trial division up to √m. */
    uint64_t primes[64];
    unsigned exps[64];
    unsigned count = 0;
    uint64_t t = m;

    for (uint64_t p = 2; p <= t / p; ++p)
    {
        if (t % p == 0)
        {
            unsigned e = 0;
            while (t % p == 0)
            {
                t /= p;
                ++e;
            }
            primes[count] = p;
            exps[count]   = e;
            ++count;
        }
    }
    if (t > 1)
    {
        primes[count] = t;
        exps[count]   = 1;
        ++count;
    }

    /* For each ordered pair of distinct primes (p,q), test condition. */
    for (unsigned i = 0; i < count; ++i)
    {
        uint64_t p = primes[i];
        unsigned e = exps[i];
        for (unsigned j = 0; j < count; ++j)
        {
            if (i == j)
                continue;
            uint64_t q = primes[j];

            uint64_t val = 1 % q;
            for (unsigned k = 1; k <= e; ++k)
            {
                val = (uint64_t)(((__int128)val * p) % q);
                if (val == 1)
                    return false;      /* violates definition */
            }
        }
    }
    return true;
}

/* Static cache of found nilpotent numbers. */
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;
static uint64_t counter = 0;      /* integer currently being tested */

/* Ensure cache can hold at least 'need' elements. */
static void ensure_cache(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = mem_new(seq_t, new_cap);
    if (!tmp)
        abort();           /* out of memory */
    for (size_t i = 0; i < cache_len; ++i)
        tmp[i] = cache[i];
    mem_free(cache);
    cache = tmp;
    cache_cap = new_cap;
}

/* Main sequence function.  Returns the n-th nilpotent number (n ≥ 1). */
seq_t a(idx_t n)
{
    /* Guard against zero index – undefined, but return 0. */
    if (n == 0)
        return (seq_t)0;

    while (cache_len < (size_t)n)
    {
        ++counter;
        if (is_nilpotent(counter))
        {
            ensure_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)counter;
        }
    }
    return cache[n - 1];
}
