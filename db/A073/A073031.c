/*
 * A073031  Number of ways of making change for n cents using coins of sizes
 *          1, 2, 5, 10 cents, when order matters.
 * https://oeis.org/A073031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-5)+a(n-10), a(0)=1
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A073031: number of ordered coin change ways using 1,2,5,10 cents */
#include <stdlib.h>

static seq_t *cache = NULL;      /* Dynamic cache for computed values */
static idx_t cache_size = 0;     /* Number of valid elements in cache */
static idx_t cache_capacity = 0; /* Current allocated capacity */

seq_t a(idx_t n) {
    /* Expand cache if needed */
    if (n + 1 > cache_capacity) {
        idx_t new_cap = n + 1;
        seq_t *res = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!res) { /* Allocation failed; return 0 as a safe fallback */
            return (seq_t)0;
        }
        cache = res;
        cache_capacity = new_cap;
    }

    /* Compute missing terms up to index n */
    for (; cache_size <= n; ++cache_size) {
        if (cache_size == 0) {
            cache[0] = (seq_t)1;
            continue;
        }
        seq_t val = 0;
        if (cache_size >= 1) val += cache[cache_size - 1];
        if (cache_size >= 2) val += cache[cache_size - 2];
        if (cache_size >= 5) val += cache[cache_size - 5];
        if (cache_size >= 10) val += cache[cache_size - 10];
        cache[cache_size] = val;
    }

    return cache[n];
}
