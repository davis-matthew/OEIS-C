/*
 * A054012  Nonzero values of n read modulo (number of proper divisors of n).
 * https://oeis.org/A054012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth non‑zero value of n mod (number of proper
 *            divisors of n)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   proper_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This routine keeps an internal cache of already computed values and
   computes new ones on demand.  For each integer k it finds the number
   of proper divisors d(k) (which is τ(k)-1) and, if k mod d(k) ≠ 0,
   appends that result to the cache.  a(n) then returns the n‑th cached
   value. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/proper_divisors.h"

/* Main sequence routine.  The first time it is called the internal
   static storage is allocated on demand. */
seq_t a(idx_t n)
{
    /* Static cache of the sequence values found so far */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t last_k = 1;   /* last integer examined */

    if (n == 0) return 0;     /* undefined for n < 1, but not expected */

    /* Grow the cache until it contains n elements */
    while (cache_len < n) {
        ++last_k;

        idx_t d = proper_divisors(last_k);
        if (d == 0) continue;               /* skip numbers with 0 proper divisors */

        seq_t val = (seq_t)(last_k % d);
        if (val == 0) continue;             /* keep only non‑zero values */

        /* Ensure capacity */
        if (cache_len == cache_cap) {
            cache_cap = cache_cap == 0 ? 64 : cache_cap * 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len++] = val;
    }

    return cache[n - 1];
}
