/*
 * A033651  Trajectory of 9 under map x->x + (x-with-digits-reversed).
 * https://oeis.org/A033651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 19, u64 n <= 41, u128 n <= 91,
 *            big any n
 * Helpers:   rev_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function implements the trajectory of 9 under the map
   x → x + rev(x), where rev(x) is the decimal reversal of x.
   The sequence a(n) is defined by:
        a(0) = 9
        a(n) = a(n‑1) + rev(a(n‑1))   for n ≥ 1
   The implementation memoises all values up to the largest index
   requested, so that calls with arbitrary n (not necessarily
   increasing) are handled efficiently. */

#include <stdlib.h>
#include "../utils/rev_decimal.h"

seq_t a(idx_t n)
{
    /* ----- static cache for memoisation ----- */
    static seq_t *cache   = NULL;   /* array of generated terms */
    static size_t cache_cap = 0;    /* allocated capacity      */
    static size_t cache_len = 0;    /* number of valid entries */

    /* ----- grow the cache if needed ----- */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {            /* abort on allocation failure */
            exit((int)-1);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* ----- initialise first element if it hasn't been set ----- */
    if (cache_len == 0) {
        cache[0] = (seq_t)9;   /* a(0) = 9 */
        cache_len = 1;
    }

    /* ----- compute missing terms up to n ----- */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = cache[i - 1];
        seq_t rev = rev_decimal(val);
        cache[i] = val + rev;
    }

    cache_len = (size_t)(n + 1); /* ensure cache_len tracks the last computed index */
    return cache[n];
}
