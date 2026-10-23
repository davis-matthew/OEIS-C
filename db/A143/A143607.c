/*
 * A143607  Numerators of principal and intermediate convergents to 2^(1/2).
 * https://oeis.org/A143607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 2*a(n-2) + a(n-4) for n > 5 Initial values: a(1)=1,
 *            a(2)=3, a(3)=4, a(4)=7, a(5)=10
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 49, u64 n <= 100,
 *            u128 n <= 201, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* cache[n-1] holds a(n) for n >= 1 */
static idx_t cache_len = 0;       /* number of entries currently stored */
static idx_t cache_cap = 0;       /* allocated capacity of the cache */

static void ensure_capacity(idx_t new_cap) {
    if (new_cap > cache_cap) {
        /* double until large enough */
        if (cache_cap == 0)
            cache_cap = 8;
        while (cache_cap < new_cap)
            cache_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) {
            /* on allocation failure, abort. This should never happen in the judge. */
            abort();
        }
        cache = tmp;
    }
}

seq_t a(idx_t n) {
    if (n == 0) /* behaviour below the offset is irrelevant */
        return 0;

    /* immediate return for the first five terms */
    if (n <= 5) {
        static const seq_t first[5] = {1, 3, 4, 7, 10};
        return first[n - 1];
    }

    /* ensure cache can hold at least n elements */
    ensure_capacity(n);

    /* if we already have up to n, return */
    if (cache_len >= n)
        return cache[n - 1];

    /* compute missing terms sequentially */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t val;
        if (i <= 5) {
            static const seq_t first[5] = {1, 3, 4, 7, 10};
            val = first[i - 1];
        } else {
            /* indices in cache are zero‑based */
            seq_t a_nminus2 = cache[i - 3]; /* a(i-2) */
            seq_t a_nminus4 = cache[i - 5]; /* a(i-4) */
            val = 2 * a_nminus2 + a_nminus4;
        }
        cache[i - 1] = val;
    }

    cache_len = n;
    return cache[n - 1];
}
