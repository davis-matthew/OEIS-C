/*
 * A012880  a(n+3) = 5*a(n+2)-4*a(n+1)+a(n).
 * https://oeis.org/A012880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+3) = 5*a(n+2) - 4*a(n+1) + a(n) Initial values: a(0)=0,
 *            a(1)=1, a(2)=3
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>                      /* malloc */
#include "../utils/ensure_cache.h"       /* ensure_cache */

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t computed_max = 0;             /* highest index computed (>=2 after init) */

seq_t a(idx_t n) {
    /* handle the first three terms explicitly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;

    /* initialise the cache once */
    if (!cache) {
        cache_cap = 3;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        computed_max = 2;
    }

    /* ensure we have space for index n */
    if ((size_t)n + 1 > cache_cap) {
        ensure_cache(n, &cache, &cache_cap);
    }

    /* compute any missing values up to n */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        cache[i] = (seq_t)5 * cache[i-1] - (seq_t)4 * cache[i-2] + cache[i-3];
    }
    computed_max = n;

    return cache[n];
}
