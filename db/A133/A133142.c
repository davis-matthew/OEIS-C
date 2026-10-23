/*
 * A133142  Numbers which are both centered square and decagonal numbers.
 * https://oeis.org/A133142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+3) = 1443*a(n+2) - 1443*a(n+1) + a(n) a(1) = 1, a(2) = 1201,
 *            a(3) = 1731661
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;          /* 0‑based array of computed terms */
static idx_t  cache_size   = 0;      /* number of terms stored (cache[0] = a(1)) */
static idx_t  cache_capacity = 0;    /* allocated capacity of cache */

/* Ensure the cache has space for at least *needed* entries.  */
/* The allocation functions work for both native integers and
   arbitrary‑precision integers. */
static void ensure_capacity(idx_t needed) {
    if (needed <= cache_capacity)
        return;                      /* already enough space */

    if (cache == NULL) {
        cache = seq_alloc(needed);    /* first allocation */
    } else {
        cache = seq_realloc(cache, cache_capacity, needed);
    }
    cache_capacity = needed;
}

/* Initialise the cache with the first three known terms of the sequence. */
static void init_cache(void) {
    ensure_capacity(3);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)1201;
    cache[2] = (seq_t)1731661;
    cache_size = 3;
}

/* Return the n‑th term of the sequence, 1‑based (a(1) = 1). */
seq_t a(idx_t n) {
    if (n == 0)                /* offset 1 – undefined, but avoid crash */
        return (seq_t)0;

    if (cache == NULL)
        init_cache();

    if (n <= cache_size)      /* already computed */
        return cache[n - 1];

    /* Grow the cache if necessary and compute missing terms. */
    ensure_capacity(n);
    for (idx_t i = cache_size; i < n; ++i) {
        seq_t t1 = (seq_t)1443 * cache[i - 1];
        seq_t t2 = (seq_t)1443 * cache[i - 2];
        cache[i] = t1 - t2 + cache[i - 3];
    }
    cache_size = n;
    return cache[n - 1];
}
