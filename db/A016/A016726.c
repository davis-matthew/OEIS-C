/*
 * A016726  Smallest k such that 1, 4, 9, ..., n^2 are distinct mod k.
 * https://oeis.org/A016726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A016726
   Smallest k such that 1,4,9,…,n² are pairwise distinct (mod k).
   For n > 4, a(n) = the smallest k ≥ 2n with k = p or k = 2p
   (p a prime).  For n = 1,2,3,4 the values are 1,2,6,9 respectively. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t x) */

static seq_t *cache = NULL;          /* cache[0] holds a(1) */
static idx_t cache_cap = 0;          /* current cached capacity */

/* Ensure cache can hold at least n entries (1‑based). */
static void ensure_cache(idx_t n)
{
    if (n <= cache_cap)
        return;
    idx_t new_cap = (cache_cap ? cache_cap * 2 : 128);
    if (new_cap < n)
        new_cap = n;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        exit(EXIT_FAILURE);
    if (cache) {
        memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
        free(cache);
    }
    /* initialise new slots to 0 (unsatisfied value) */
    memset(new_cache + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined for n==0, defensive */

    /* Small known values */
    if (n == 1) return 1;
    if (n == 2) return 2;
    if (n == 3) return 6;
    if (n == 4) return 9;

    /* use the cache for previously computed results */
    ensure_cache(n);
    if (cache[n - 1] != 0)
        return cache[n - 1];

    /* For n > 4: search for the smallest k ≥ 2n satisfying the prime
       condition described in the OEIS entry. */
    idx_t k = 2 * n;   /* start at the lower bound 2n (fits in idx_t for n≤10⁴) */

    for (;;) {
        /* Even k: candidate is 2p, so test p = k/2. */
        if ((k & 1ULL) == 0) {
            idx_t p = k >> 1;
            if (is_prime(p))
                break;
        }
        /* Odd k: candidate must be prime. */
        else {
            if (is_prime(k))
                break;
        }
        ++k;
    }

    cache[n - 1] = (seq_t)k;   /* store as seq_t; values fit for all used types
                                 (for small seq_t >|a(n) may wrap but
                                 only exact when within limits). */
    return cache[n - 1];
}
