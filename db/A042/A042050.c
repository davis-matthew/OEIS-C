/*
 * A042050  Numerators of continued fraction convergents to sqrt(549).
 * https://oeis.org/A042050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = 3532638098*a(n-18) - a(n-36) with initial terms
 *            a(0)...a(35) given by the convergents to sqrt(549)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   init_36_terms
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------ */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/init_36_terms.h"

/* ----------------------------------------------------------------
   Sequence access function.
   ---------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache  = NULL;      /* cached terms */
    static size_t cap    = 0;         /* allocated size of cache      */
    static idx_t computed = 0;        /* next index not yet computed  */

    /* ------------------------------------------------------------
       1) Ensure we have space for indices [0 .. n]
       ------------------------------------------------------------ */
    if ((size_t)(n + 1) > cap) {
        size_t newcap = (size_t)(n + 1);
        /* increase a bit to reduce reallocs */
        if (newcap < 64) newcap = 64;
        else newcap <<= 1;
        if (cache == NULL) {
            cache = (seq_t*)malloc(newcap * sizeof(*cache));
        } else {
            cache = (seq_t*)realloc(cache, newcap * sizeof(*cache));
        }
        /* initialise new slots to zero – not strictly needed */
        if (cache) {
            for (size_t i = cap; i < newcap; ++i) cache[i] = 0;
        }
        cap = newcap;
    }

    /* ------------------------------------------------------------
       2) Initialise the first 36 terms once.
       ------------------------------------------------------------ */
    if (computed == 0) {
        if (cap < 36) {
            /* grow to at least 36 */
            size_t newcap = 64;
            if (cache == NULL) cache = (seq_t*)malloc(newcap * sizeof(*cache));
            else cache = (seq_t*)realloc(cache, newcap * sizeof(*cache));
            cap = newcap;
        }
        init_36_terms(cache);
        computed = 36; /* indices 0..35 are valid */
    }

    /* ------------------------------------------------------------
       3) Return a cached value if we already have it.
       ------------------------------------------------------------ */
    if (n < computed) {
        return cache[n];
    }

    /* ------------------------------------------------------------
       4) Compute remaining terms using linear recurrence.
          a(n) = 3532638098 * a(n-18) - a(n-36)
       ------------------------------------------------------------ */
    const seq_t COEFF = (seq_t)3532638098ULL;
    for (idx_t i = computed; i <= n; ++i) {
        seq_t prod = COEFF * cache[i - 18];
        cache[i] = prod - cache[i - 36];
    }
    computed = n + 1;

    return cache[n];
}
