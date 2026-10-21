/*
 * A233302  Number of (2+1) X (n+1) 0..1 arrays x(i,j) with row sums
 *          sum{x(i,j), j=1..n+1} nondecreasing, and column sums
 *          sum{i^2*x(i,j), i=1..2+1} nondecreasing.
 * https://oeis.org/A233302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 3*a(n-2) - 7*a(n-3) + 10*a(n-4) + 3*a(n-5) * -
 *            6*a(n-6) - 6*a(n-7) + 3*a(n-8) + 10*a(n-9) - 7*a(n-10) * -
 *            3*a(n-11) + 4*a(n-12) - a(n-13) * initial values: a(1)=15,
 *            a(2)=42, a(3)=105, a(4)=232, a(5)=475, * a(6)=904, a(7)=1632,
 *            a(8)=2806, a(9)=4642, a(10)=7414, * a(11)=11500, a(1...
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 98, u64 n <= 172, u128 n <= 172,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

static const seq_t init_vals[14] = {
    0,          /* dummy for 0-index */
    15, 42, 105, 232, 475, 904, 1632, 2806,
    4642, 7414, 11500, 17368, 25636
};

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_size = 0;

static void grow_cache(idx_t new_cap) {
    if (new_cap > cache_cap) {
        size_t new_size = (cache_cap == 0) ? 64 : cache_cap * 2;
        while (new_size < new_cap) new_size *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort(); /* allocation failure */
        cache = tmp;
        cache_cap = new_size;
    }
}

seq_t a(idx_t n) {
    if (n <= 13) return init_vals[n];

    /* initialize cache if not yet done */
    if (!cache) {
        grow_cache(64);
        /* copy initial values into cache */
        for (idx_t i = 1; i <= 13; ++i) cache[i] = init_vals[i];
        cache_size = 13;
    }

    /* ensure cache capacity for requested index */
    grow_cache(n + 1);

    /* compute missing terms up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t pos = 0, neg = 0;

        /* positive contributions */
        pos += mul_small(cache[i-1], 4U);
        pos += mul_small(cache[i-4], 10U);
        pos += mul_small(cache[i-5], 3U);
        pos += mul_small(cache[i-8], 3U);
        pos += mul_small(cache[i-9], 10U);
        pos += mul_small(cache[i-12], 4U);

        /* negative contributions */
        neg += mul_small(cache[i-2], 3U);
        neg += mul_small(cache[i-3], 7U);
        neg += mul_small(cache[i-6], 6U);
        neg += mul_small(cache[i-7], 6U);
        neg += mul_small(cache[i-10], 7U);
        neg += mul_small(cache[i-11], 3U);
        neg += mul_small(cache[i-13], 1U);

        cache[i] = pos - neg;  /* pos > neg for this sequence */
    }

    cache_size = n; /* update known size */
    return cache[n];
}
