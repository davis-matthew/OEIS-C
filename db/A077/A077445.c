/*
 * A077445  Numbers k such that (k^2 - 8)/2 is a square.
 * https://oeis.org/A077445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - a(n-2), a(1) = 4, a(2) = 20 (linear
 *            recurrence, solution 4*(3+2√2)^(n-1)+4*(3-2√2)^(n-1)/2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;    /* cache of a(1) .. a(k) */
static size_t cache_allocated = 0;
static idx_t  cache_size = 0;  /* number of terms stored (>=2 after init) */

/* ensure the cache can hold at least `needed` elements */
static void ensure_capacity(idx_t needed) {
    if (needed <= cache_allocated) return;
    size_t new_allocated = cache_allocated ? cache_allocated * 2 : 2;
    if (new_allocated < needed) new_allocated = needed;
    seq_t *new_cache = (seq_t*)realloc(cache, new_allocated * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;
    cache_allocated = new_allocated;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;          /* not in the OEIS range */

    /* Initialize base cases if needed */
    if (cache_size < 2) {
        ensure_capacity(2);
        cache[0] = (seq_t)4;             /* a(1) */
        cache[1] = (seq_t)20;            /* a(2) */
        cache_size = 2;
    }

    /* Return already computed value */
    if (n <= cache_size)
        return cache[n-1];

    /* Populate the cache up to index n */
    ensure_capacity(n);
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        idx_t idx = i - 1;                /* zero‑based index for a(i) */
        /* For i >= 3, recurrence holds: a(i) = 6*a(i-1) - a(i-2) */
        seq_t val = (seq_t)6 * cache[idx-1];
        val = val - cache[idx-2];
        cache[idx] = val;
    }

    cache_size = n;
    return cache[n-1];
}
