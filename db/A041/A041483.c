/*
 * A041483  Denominators of continued fraction convergents to sqrt(258).
 * https://oeis.org/A041483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 514*a(n-2) - a(n-4) for n > 3 Initial values: a(0) = 1
 *            a(1) = 16 a(2) = 513 a(3) = 8224 Sequence is memoised to allow
 *            out‑of‑order calls up to n ~ 10000
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* capacity of cache array */
static idx_t  cache_len = 0;   /* number of elements stored in cache */

/* initial values (cannot be static const if seq_t is an arbitrary‑precision
   class, but most compilers accept const for such types). */
static const seq_t init_vals[4] = {
    (seq_t)1,
    (seq_t)16,
    (seq_t)513,
    (seq_t)8224
};

/* Allocate and initialise cache if it does not yet exist. */
static void ensure_cache(void) {
    if (cache != NULL)
        return;
    cache_cap = 4;
    cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    if (!cache)
        abort();
    for (idx_t i = 0; i < 4; ++i)
        cache[i] = init_vals[i];
    cache_len = 4;
}

/* Grow the cache so that it contains at least indices 0..n (inclusive). */
static void grow_cache_to(idx_t n) {
    if (cache_len > n)
        return;

    /* Double the capacity until we can store at least n+1 elements. */
    while (cache_len <= n) {
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap << 1 : 8;
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
            if (!tmp)
                abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        idx_t k = cache_len;          /* index of the element to compute */
        seq_t prod = (seq_t)514;      /* 514 as seq_t */
        prod *= cache[k - 2];         /* 514 * a(k-2) */
        seq_t val = prod - cache[k - 4];  /* 514*a(k-2) - a(k-4) */
        cache[cache_len] = val;
        ++cache_len;
    }
}

/* External linkage function required by the test harness. */
seq_t a(idx_t n) {
    if (n < 4)
        return init_vals[n];

    ensure_cache();
    if (cache_len <= n)
        grow_cache_to(n);

    return cache[n];
}
