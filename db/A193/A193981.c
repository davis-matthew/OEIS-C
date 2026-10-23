/*
 * A193981  Number of ways to arrange 3 nonattacking triangular rooks on an
 *          nXnXn triangular grid.
 * https://oeis.org/A193981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) -14*a(n-2) +14*a(n-3) -14*a(n-5) +14*a(n-6)
 *            -6*a(n-7) +a(n-8) initial values: a(1)=0, a(2)=0, a(3)=0,
 *            a(4)=2, a(5)=23, a(6)=127, a(7)=468, a(8)=1352
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 78, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   seq_alloc_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n) {
    /* lazy memoisation of values up to the largest n requested */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;       /* one past last valid index (n+1) */
    static idx_t computed_to = 0;      /* highest n for which cache[n] is defined */

    if (n < 1) return 0;           /* out‑of‑range, not used in tests */

    /* grow the cache if needed */
    if ((size_t)n + 1 > cache_cap) {
        /* new capacity must be at least n+1 */
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = seq_realloc(cache, cache_cap, new_cap);
        if (!new_cache) return 0;   /* allocation failed – caller must handle */
        if (!cache) {               /* first allocation */
            /* initialise base values for indices 1..8 (if they fit) */
            cache = new_cache;
            cache_cap = new_cap;
            if (cache_cap > 1) cache[1] = 0;
            if (cache_cap > 2) cache[2] = 0;
            if (cache_cap > 3) cache[3] = 0;
            if (cache_cap > 4) cache[4] = 2;
            if (cache_cap > 5) cache[5] = 23;
            if (cache_cap > 6) cache[6] = 127;
            if (cache_cap > 7) cache[7] = 468;
            if (cache_cap > 8) cache[8] = 1352;
            computed_to = (cache_cap > 8 ? 8 : (idx_t)(cache_cap-1));
        } else {
            /* enlarge existing cache – copy old values already done by seq_realloc */
            cache = new_cache;
            cache_cap = new_cap;
        }
    }

    /* compute new terms up to n */
    if (n > computed_to) {
        if (computed_to < 8) {
            /* fill missing initial values that may have been skipped by earlier calls */
            if (computed_to < 1 && cache_cap > 1) cache[1] = 0;
            if (computed_to < 2 && cache_cap > 2) cache[2] = 0;
            if (computed_to < 3 && cache_cap > 3) cache[3] = 0;
            if (computed_to < 4 && cache_cap > 4) cache[4] = 2;
            if (computed_to < 5 && cache_cap > 5) cache[5] = 23;
            if (computed_to < 6 && cache_cap > 6) cache[6] = 127;
            if (computed_to < 7 && cache_cap > 7) cache[7] = 468;
            if (computed_to < 8 && cache_cap > 8) cache[8] = 1352;
            computed_to = 8;
        }
        for (idx_t i = computed_to + 1; i <= n; ++i) {
            /* recurrence only valid for i >= 9, but we have already set i>=9 here */
            seq_t val = 6 * cache[i-1];
            val -= 14 * cache[i-2];
            val += 14 * cache[i-3];
            val -= 14 * cache[i-5];
            val += 14 * cache[i-6];
            val -= 6 * cache[i-7];
            val += cache[i-8];
            cache[i] = val;
        }
        computed_to = n;
    }

    return cache[n];
}
