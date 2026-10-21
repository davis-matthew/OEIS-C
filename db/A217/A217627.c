/*
 * A217627  a(n) is the sum of the products of the nonzero digits of the
 *          numbers from 1 to n.
 * https://oeis.org/A217627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum_{k=1..n} digit_product(k) (OEIS A217627)
 * Range:     u8 n <= 36, u16 n <= 851, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Compute the product of the non‑zero decimal digits of x. */
static seq_t digit_product(idx_t x) {
    seq_t prod = 1;
    while (x > 0) {
        unsigned d = x % 10;
        if (d != 0)
            prod = prod * (seq_t)d;
        x /= 10;
    }
    return prod;
}

/* Prefix‑sum cache.  cache[i] == a(i).  cache[0] is unused. */
static seq_t *cache = NULL;   /* cached values */
static idx_t  cache_cap = 0;   /* allocated size (number of elements) */
static idx_t  cache_size = 0;  /* largest index computed */

/* Ensure the cache can hold at least indices up to need.  keep 0 unused. */
static void extend_cache(idx_t need) {
    /* already enough space? */
    if (need < cache_cap)
        return;

    /* new capacity: double size or at least need+1. */
    idx_t new_cap = (cache_cap > 0) ? cache_cap * 2 : 16;
    if (new_cap <= need)
        new_cap = need + 1;

    seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_cap);
    if (!new_cache)
        abort();                     /* allocation failure */

    /* copy old data */
    if (cache) {
        for (idx_t i = 0; i <= cache_size; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }

    /* initialise the unused element 0 to 0 (undefined for the sequence) */
    new_cache[0] = 0;
    cache = new_cache;
    cache_cap = new_cap;
}

/* OEIS A217627: sum of digit products from 1 to n. */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;   /* undefined for the sequence, but harmless */

    if (n > cache_size) {
        extend_cache(n);
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            seq_t prod = digit_product(i);
            cache[i] = cache[i - 1] + prod;
        }
        cache_size = n;
    }
    return cache[n];
}
