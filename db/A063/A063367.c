/*
 * A063367  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          94 ).
 * https://oeis.org/A063367
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-4) + a(n-6) - a(n-10) for n > 13, with a(2) = -1,
 *            a(3) = 91, a(4) = 184, a(5) = 276, a(6) = 368, a(7) = 460, a(8)
 *            = 552, a(9) = 642, a(10) = 736, a(11) = 826, a(12) = 920, a(13)
 *            = 1012 (offset 2)
 * Range:     i8 n <= 3, i16 n <= 43, i32 n <= 43, i64 n <= 43, i128 n <= 43,
 *            big any n
 * Helpers:   alloc_cache, free_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_cache.h"   /* provides seq_t *alloc_cache(idx_t) */
#include "../utils/free_cache.h"    /* provides void free_cache(seq_t*) */

static seq_t *cache = NULL;      /* 0‑indexed, indices 0 and 1 are unused */
static size_t cap = 0;           /* current cache size (= highest index + 1) */

/* initial values for indices 0…13 (indices 0 and 1 are set to 0) */
static const seq_t init_vals[14] = {
    /* 0 */   0,
    /* 1 */   0,
    /* 2 */  -1,
    /* 3 */   91,
    /* 4 */  184,
    /* 5 */  276,
    /* 6 */  368,
    /* 7 */  460,
    /* 8 */  552,
    /* 9 */  642,
    /*10 */  736,
    /*11 */  826,
    /*12 */  920,
    /*13 */ 1012
};

seq_t a(idx_t n) {
    if (n < 2) return 0;   /* values below the offset are irrelevant */

    /* grow the cache if necessary */
    if (cap <= (size_t)n) {
        size_t oldcap = cap;
        size_t newcap = (size_t)n + 1;
        seq_t *new_cache = alloc_cache((idx_t)newcap);
        for (size_t i = 0; i < oldcap; ++i) new_cache[i] = cache[i];
        if (cache != NULL) free_cache(cache);
        cache = new_cache;
        cap   = newcap;

        /* fill from the old capacity up to n */
        for (size_t i = oldcap; i <= (size_t)n; ++i) {
            if (i < 2) {
                cache[i] = 0;
            } else if (i <= 13) {
                cache[i] = init_vals[i];
            } else {
                cache[i] = cache[i - 4] + cache[i - 6] - cache[i - 10];
            }
        }
    } else {
        /* cache already large enough, just ensure any missing entries are filled */
        for (size_t i = cap; i <= (size_t)n; ++i) {
            if (i < 2) {
                cache[i] = 0;
            } else if (i <= 13) {
                cache[i] = init_vals[i];
            } else {
                cache[i] = cache[i - 4] + cache[i - 6] - cache[i - 10];
            }
        }
        cap = (size_t)n + 1;   /* no need to change the allocated size */
    }

    return cache[(size_t)n];
}
