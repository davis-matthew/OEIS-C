/*
 * A011934  a(n) = |1^3 - 2^3 + 3^3 - 4^3 + ... + (-1)^(n+1)*n^3|.
 * https://oeis.org/A011934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0; a(n) = |1^3 - 2^3 + ... + (-1)^(n+1)*n^3|
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = n^3 - a(n-1) (for n ≥ 1). */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache to allow repeated calls in any order. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* highest index currently stored */
    static idx_t cache_size = 0;  /* allocated size of cache */

    /* Ensure cache is initialized with a(0). */
    if (!cache) {
        cache_size = 1;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_size);
        if (!cache) return 0; /* allocation failed – return 0 as a safe fallback */
        cache[0] = 0;
        cache_len = 0;
    }

    /* If requested n is already computed, return it directly. */
    if (n <= cache_len)
        return cache[n];

    /* Grow cache if necessary. */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp) return 0; /* realloc failed – return 0 */
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t iS = (seq_t)i;
        seq_t i2 = iS * iS;
        seq_t i3 = i2 * iS;          /* i^3 */
        seq_t prev = cache[i - 1];
        seq_t val;
        if (prev > i3)
            val = prev - i3;         /* in case of unexpected negative */
        else
            val = i3 - prev;
        cache[i] = val;
    }

    cache_len = n;
    return cache[n];
}
