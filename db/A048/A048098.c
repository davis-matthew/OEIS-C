/*
 * A048098  Numbers k that are sqrt(k)-smooth: if p | k then p^2 <= k when p
 *          is prime.
 * https://oeis.org/A048098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 4367, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A048098: a(n) is the n‑th integer whose prime factors all satisfy
   p^2 ≤ k (equivalently, the maximal prime factor of k is ≤ √k). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "../utils/floor_sqrt.h"
#include "../utils/build_primes_upto.h"

/* ------------------------------------------------------------------ */
/* State used by the function                                         */
/* ------------------------------------------------------------------ */
static seq_t *cache      = NULL;  /* stored terms of the sequence     */
static size_t cache_len  = 0;     /* number of terms cached          */
static size_t cache_cap  = 0;     /* allocated cache capacity        */

static uint64_t *primes   = NULL;  /* primes up to current limit      */
static size_t   primes_cnt = 0;     /* number of primes in the array   */
static idx_t    prime_limit = 0;    /* maximum value covered by primes */

/* ------------------------------------------------------------------ */
/* Factorise k using primes ≤ √k.  Return true iff all prime factors
   of k satisfy p ≤ √k (i.e., k is sqrt‑smooth).                     */
/* ------------------------------------------------------------------ */
static bool is_sqrt_smooth(seq_t k)
{
    /* Compute integer floor √k (returns seq_t) and cast to idx_t */
    idx_t sqrtk = (idx_t)floor_sqrt(k);

    /* Ensure we have primes up to sqrtk */
    if (sqrtk > prime_limit) {
        /* Re‑build prime table */
        if (primes) free(primes);
        build_primes_upto(sqrtk, &primes, &primes_cnt);
        prime_limit = sqrtk;
    }

    seq_t r = k;                /* copy of k to work on                 */
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if ((idx_t)p > sqrtk) break;   /* primes beyond √k are irrelevant */

        /* Remove all copies of p from r */
        while ((r % (seq_t)p) == 0) {
            r /= (seq_t)p;
        }
    }

    /* If a residue > 1 remains, it has a prime factor > √k */
    return !(r > (seq_t)1);
}

/* ------------------------------------------------------------------ */
/* Ensure that the cache contains at least 'needed' terms.  Extend
   the cache by searching for additional sqrt‑smooth numbers.           */
/* ------------------------------------------------------------------ */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_len) return;

    /* Grow the cache storage if necessary */
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();              /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Start searching from the next integer after the last cached value */
    idx_t cur = (cache_len == 0) ? 1 : (idx_t)cache[cache_len - 1] + 1;

    /* Find candidates until we hit the required count */
    while (cache_len < needed) {
        seq_t candidate = (seq_t)cur;
        if (is_sqrt_smooth(candidate)) {
            cache[cache_len++] = candidate;
        }
        ++cur;
    }
}

/* ------------------------------------------------------------------ */
/* Return the n‑th term of the sequence (1‑based indexing).            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return 0;            /* undefined for n < 1 */

    size_t needed = (size_t)n;
    ensure_cache(needed);
    return cache[n - 1];
}
