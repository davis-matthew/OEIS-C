/*
 * A160128  a(n) = number of grid points that are covered after (2^n)th stage
 *          of A139250.
 * https://oeis.org/A160128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/9)*(2^(2*n+3) + 12*n + 19)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 6*a(n-1) - 9*a(n-2) + 4*a(n-3), with a(0)=3, a(1)=7, a(2)=19 */
/* This implementation uses the recurrence, memoising previously computed values.
 * For C builds (seq_t is an unsigned integer type) intermediate products are
 * computed in an unsigned 64‑bit type so no overflow occurs before the final
 * cast.  For C++ builds (seq_t may be an arbitrary‑precision integer class)
 * the recurrence is evaluated directly with seq_t operations.  The function
 * works for any n ≥ 0 and handles calls in non‑increasing order of n.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t   cache_size = 0;   /* number of terms stored (next index to fill) */

    /* initialise cache with first three terms */
    if (cache == NULL) {
        cache = seq_realloc(NULL, 0, 3);
        cache_cap = 3;
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)7;
        cache[2] = (seq_t)19;
        cache_size = 3;
    }

    /* grow cache if necessary */
    if ((idx_t)cache_size <= n) {
        /* ensure capacity */
        idx_t needed = n + 1;
        if ((size_t)needed > cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap < (size_t)needed) new_cap = (size_t)needed;
            cache = seq_realloc(cache, cache_cap, new_cap);
            cache_cap = new_cap;
        }

        /* compute missing terms up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
#ifdef __cplusplus
            /* seq_t is a (possibly arbitrary‑precision) integer class */
            cache[i] = 6 * cache[i - 1]
                     - 9 * cache[i - 2]
                     + 4 * cache[i - 3];
#else
            /* seq_t is an unsigned integer type – use a wider type for safety */
            unsigned long long t1 = (unsigned long long)cache[i - 1];
            unsigned long long t2 = (unsigned long long)cache[i - 2];
            unsigned long long t3 = (unsigned long long)cache[i - 3];
            unsigned long long val = 6ULL * t1
                                   - 9ULL * t2
                                   + 4ULL * t3;
            cache[i] = (seq_t)val;
#endif
        }
        cache_size = n + 1;
    }
    return cache[n];
}
