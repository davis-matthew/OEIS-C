/*
 * A039546  Numbers whose base-12 representation has the same nonzero number
 *          of 6's and 9's.
 * https://oeis.org/A039546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   has_equal_six_nine
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose base‑12 representation has the same
 * non‑zero number of 6's and 9's.
 *
 * Computation: iterate over consecutive integers, test the
 * property in base 12, and store the results in a static cache.
 * The function returns the cached value for any requested index.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/has_equal_six_nine.h"

static seq_t *cache = NULL;        /* dynamic array of seq_t */
static size_t cacheSize = 0;      /* number of values stored */
static size_t cacheCap = 0;       /* allocated capacity of cache */
static seq_t nextCandidate = (seq_t)1;  /* next integer to test */

static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cacheCap)
        return;
    size_t newCap = cacheCap ? cacheCap * 2 : 64;
    while (newCap < needed)
        newCap *= 2;
    seq_t *newCache = (seq_t *)malloc(newCap * sizeof(seq_t));
    if (!newCache) {
        /* In an environment where allocation fails, we cannot continue.
           Abort since the sequence cannot be produced. */
        abort();
    }
    if (cache) {
        /* copy existing values */
        for (size_t i = 0; i < cacheSize; ++i)
            newCache[i] = cache[i];
        free(cache);
    }
    cache = newCache;
    cacheCap = newCap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for offset 1, but safe return 0 */

    /* Ensure we have enough precomputed values. */
    while (cacheSize < (size_t)n) {
        /* Find next number satisfying the property. */
        seq_t val = nextCandidate;
        for (;;) {
            if (has_equal_six_nine(val)) {
                /* Store it and prepare for next search. */
                ensure_cache_capacity(cacheSize + 1);
                cache[cacheSize++] = val;
                nextCandidate = val + (seq_t)1;
                break;
            }
            ++val;
        }
    }

    return cache[n - 1];
}
