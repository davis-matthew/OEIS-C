/*
 * A186469  Number of (n+1) X 7 0..2 arrays with every 2 X 2 subblock
 *          commuting with each of its horizontal and vertical 2 X 2 subblock
 *          neighbors.
 * https://oeis.org/A186469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)+a(n-2)-4*a(n-3)+4*a(n-4)-2*a(n-5) for n>15
 *            Initial values: a(1)=1321, a(2)=1754, a(3)=4023, a(4)=9834,
 *            a(5)=16083, a(6)=34314, a(7)=49407, a(8)=109950, a(9)=159879,
 *            a(10)=350418, a(11)=540777, a(12)=1143302, a(13)=1845649,
 *            a(14)=3753244, a(15)=6241282
 * Range:     u8 none, u16 n <= 7, u32 n <= 25, u64 n <= 62, u128 n <= 136,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[15] = {
    1321, 1754, 4023, 9834, 16083,
    34314, 49407, 109950, 159879,
    350418, 540777, 1143302, 1845649,
    3753244, 6241282
};

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void ensure_cache_capacity(size_t new_cap) {
    if (new_cap > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Compute the A186469 term at 1‑based index n. */
seq_t a(idx_t n) {
    if (n <= 15) return init_vals[n - 1];

    /* Initialise memoisation storage on first use. */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        for (size_t i = 0; i < 15; ++i) cache[i] = init_vals[i];
        cache_len = 15;
    }

    /* Return if already computed. */
    if ((size_t)n <= cache_len) return cache[n - 1];

    /* Grow capacity if necessary. */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap << 1;
        while (new_cap < (size_t)n) new_cap <<= 1;
        ensure_cache_capacity(new_cap);
    }

    /* Compute missing terms up to n. */
    for (size_t k = cache_len; k < (size_t)n; ++k) {
        /* k is 0‑based index for term k+1. */
        cache[k] = 2 * cache[k - 1] + cache[k - 2]
                 - 4 * cache[k - 3] + 4 * cache[k - 4]
                 - 2 * cache[k - 5];
    }
    cache_len = n;
    return cache[n - 1];
}
