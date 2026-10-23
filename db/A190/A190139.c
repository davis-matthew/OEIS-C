/*
 * A190139  a(n) = 2*a(n-1) + a(n-2) + a(n-3) + a(n-4), a(-2)=0, a(-1)=0,
 *          a(0)=1, a(1)=1.
 * https://oeis.org/A190139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + a(n-3) + a(n-4); a(0) = 1, a(1) = 1,
 *            a(-2) = a(-1) = 0
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function keeps a cache that grows as needed and can be called in any order. */
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;          /* allocated elements */
static idx_t max_computed = 0;        /* highest index for which a value is stored */

static void ensure_cache(idx_t n) {
    if (cache == NULL) {
        cache_cap = 4; /* at least big enough for indices 0..3 when needed */
        cache = (seq_t*)malloc(sizeof(seq_t)*cache_cap);
        if (!cache) return; /* allocation failed – let caller handle zero */
        /* initial known values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        /* not storing a(-1), a(-2); they are implicitly zero by using no entry */
        max_computed = 1;
    }
    if (n <= max_computed) return;
    /* Grow the cache to at least n+1 */
    while (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap << 1;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t)*new_cap);
        if (!tmp) return; /* allocation failed – abort */
        cache = tmp;
        cache_cap = new_cap;
    }
    /* Compute missing values */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t val;
        if (i == 2) {
            /* a(-2) and a(-1) are zero */
            val = ((seq_t)2)*cache[1] + cache[0];
        } else if (i == 3) {
            /* a(-1) and a(-2) are zero */
            val = ((seq_t)2)*cache[2] + cache[1] + cache[0];
        } else {
            val = ((seq_t)2)*cache[i-1] + cache[i-2] + cache[i-3] + cache[i-4];
        }
        cache[i] = val;
    }
    max_computed = n;
}

seq_t a(idx_t n) {
    if (n == (idx_t)((-2) | 0)) return 0; /* impossible normal idx_t so just guard */
    if (n <= 1) return (seq_t)(n==0?1:1);
    ensure_cache(n);
    return cache[n];
}
