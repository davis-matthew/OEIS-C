/*
 * A038955  Primes p such that 71 is a square mod p.
 * https://oeis.org/A038955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow, seq_cmp, seq_from_str, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes p such that 71 is a quadratic residue modulo p (Legendre
   symbol (71|p) = 1).  The sequence starts with 2,5,7,11,23,29,31,37,47,
   59,67,71,73,89,… */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/is_prime.h"   /* bool is_prime(idx_t) */
#include "../utils/mod_pow.h"    /* seq_t mod_pow(seq_t, uint64_t, seq_t) */
#include "../utils/seq_realloc.h"/* seq_t *seq_realloc(seq_t*, size_t, size_t) */
#include "../utils/seq_cmp.h"     /* int seq_cmp(const void*, const void*) */
#include "../utils/seq_from_str.h"/* seq_t seq_from_str(const char*) */

/* --- Cached sequence storage -------------------------------------------- */

static seq_t *cache        = NULL;   /* dynamic array of sequence terms     */
static size_t cache_size   = 0;      /* number of terms currently stored    */
static size_t cache_cap    = 0;      /* allocated capacity of cache         */
static idx_t last_checked  = 1;      /* last integer examined in sieve      */

/* Ensure the cache can hold at least `need` elements (1‑based indexing). */
static void
ensure_cache_capacity(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < need)
        new_cap <<= 1;
    cache = seq_realloc(cache, cache_cap, new_cap);
    cache_cap = new_cap;
}

/* Append a new term to the cache (assumes capacity already ensured). */
static inline void
cache_append(seq_t v)
{
    cache[cache_size++] = v;
}

/* --- Main sequence function --------------------------------------------- */

/* Return the nth term (n >= 1).  For n == 0 the function returns 0 (undefined
   offset for this OEIS entry). */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;                       /* undefined for offset 1 */

    size_t want = (size_t)n;             /* desired number of terms */

    /* Extend cache until we have the requested term. */
    if (cache_size < want) {
        ensure_cache_capacity(want);

        for (idx_t cand = last_checked + 1;
             cache_size < want;
             ++cand)
        {
            /* Skip non‑prime candidates. */
            if (!is_prime(cand))
                continue;

            /* 2 and 71 are special cases: they are in the sequence but
               the Legendre symbol is not defined in the usual way. */
            if (cand == 2 || cand == 71) {
                cache_append((seq_t)cand);
                last_checked = cand;
                continue;
            }

            /* Legendre symbol (71|cand) = 1  <=>  71^((cand-1)/2) ≡ 1 (mod cand). */
            uint64_t exp = (cand - 1) / 2;
            seq_t leg = mod_pow(71ULL, exp, cand);

            /* Compare result with 1. */
            seq_t one = seq_from_str("1");
            if (seq_cmp(&leg, &one) == 0)
                cache_append((seq_t)cand);

            last_checked = cand;
        }
    }

    return cache[n - 1];
}
