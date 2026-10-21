/*
 * A070864  a(1) = a(2) = 1; a(n) = 2 + a(n - a(n-1)).
 * https://oeis.org/A070864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = a(2) = 1; for n > 2, a(n) = 2 + a(n - a(n-1))
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Memoization state */
    static seq_t *cache = NULL;   /* array of terms (1‑based indexing) */
    static size_t cap = 0;        /* current array capacity */
    static idx_t max_calculated = 0; /* highest n for which cache is valid */

    if (n == 0) return 0; /* undefined for 0, but return 0 to avoid UB */

    /* If we already have the term, return it */
    if (n <= max_calculated)
        return cache[n - 1];

    /* Grow the cache if necessary */
    if ((size_t)n > cap) {
        size_t new_cap = cap ? cap * 2 : 16;
        while (new_cap < (size_t)n) new_cap *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();  /* out of memory */
        cache = new_cache;
        cap = new_cap;
    }

    /* Compute terms from (max_calculated + 1) up to n */
    for (idx_t i = max_calculated + 1; i <= n; ++i) {
        if (i == 1 || i == 2) {
            cache[i - 1] = (seq_t)1;
        } else {
            idx_t prev_val = (idx_t)cache[i - 2];          /* a(i-1) */
            idx_t j = i - prev_val;                       /* index inside cache */
            cache[i - 1] = (seq_t)2 + cache[j - 1];       /* a(i) = 2 + a(j) */
        }
    }

    max_calculated = n;
    return cache[n - 1];
}
