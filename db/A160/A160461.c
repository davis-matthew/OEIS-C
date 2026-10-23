/*
 * A160461  Coefficients in the expansion of C/B^2, in Watson's notation of
 *          page 106.
 * https://oeis.org/A160461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the coefficient of x^n in the series 1 / ∏_{m≥1} (1 -
 *            x^(m/gcd(m,5)))^(–1). Equivalently, a(0)=1 and for n>0 a(n)=∑_{k
 *            = 1}^{n} c(k)·a(n−k) with c(k)=2 if k%5≠0, otherwise 1. The
 *            implemented algorithm expands the product directly: for each
 *            part size m, if m is a multiple of 5 it is proces...
 * Range:     u8 n <= 8, u16 n <= 23, u32 n <= 70, u64 n <= 230, u128 n <= 808,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cached values: cache[n] holds a(n) for 0 ≤ n < cache_size.
       Whenever a larger n is requested, recompute the whole sequence up
       to that limit.  The computation runs in O(n²) time, which is well
       below the 2‑second budget for n ≤ 10 000. */
    static seq_t *cache   = NULL;
    static size_t cache_size = 0;

    if (n >= cache_size) {
        /* Allocate space for new terms. */
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache = new_cache;
        /* Initialise all entries to zero before recomputation. */
        for (size_t i = 0; i < new_size; ++i)
            cache[i] = 0;
        cache[0] = 1;  /* a(0) = 1 */

        /* Expand the generating function:
           For each part size m, if it is a multiple of 5 we apply the
           part once, otherwise twice (two independent colours). */
        for (idx_t m = 1; m <= n; ++m) {
            size_t times = ((m % 5) == 0) ? 1 : 2;
            for (size_t t = 0; t < times; ++t) {
                for (idx_t s = m; s <= n; ++s) {
                    cache[s] += cache[s - m];
                }
            }
        }
        cache_size = new_size;
    }
    return cache[n];
}
