/*
 * A139319  a(n) = the smallest value of the form n*k - 1, k = positive
 *          integer, that is coprime to all the previous terms of this
 *          sequence. a(1)=1.
 * https://oeis.org/A139319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the smallest k such that n*k‑1 is coprime to all
   previous terms of the sequence.  The sequence is memoised so that
   calls may be in any order.  */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;          /* cache[0] = a(1), cache[1] = a(2), ... */
static size_t cache_len = 0;         /* number of cached terms */
static size_t cache_cap = 0;         /* allocated capacity of cache array */

/* Allocate enough space for at least 'needed' elements in the cache. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Return the nth term of OEIS A139319 (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined, never called */

    /* a(1) and a(2) are both 1.  Initialise the cache if needed. */
    if (n <= 2) return 1;
    if (cache_len < 2) {
        ensure_cache_capacity(2);
        cache[0] = 1;
        cache[1] = 1;
        cache_len = 2;
    }

    /* Compute missing terms up to n. */
    for (idx_t m = cache_len + 1; m <= n; ++m) {
        seq_t cand;
        /* Find the smallest k ≥ 1 such that m*k‑1 is coprime to all
           a(1)…a(m‑1). */
        for (idx_t k = 1; ; ++k) {
            cand = (seq_t)m * k - 1;
            bool ok = true;
            for (size_t i = 0; i < (size_t)m - 1; ++i) {
                if (gcd(cand, cache[i]) > (seq_t)1) { ok = false; break; }
            }
            if (ok) break;    /* candidate found */
        }
        ensure_cache_capacity(cache_len + 1);
        cache[cache_len++] = cand;
    }

    return cache[n - 1];
}
